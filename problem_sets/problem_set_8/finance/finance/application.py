import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # Select all of the users transactions, grouping them quotes
    rows = db.execute("SELECT quotesymbol, SUM(amount) AS shares FROM transactions WHERE userid = :userid GROUP BY quotesymbol", userid=session["user_id"])

    cash = db.execute("SELECT cash FROM users WHERE id=:id", id=session["user_id"])

    portfolio = []
    balance = 0
    for row in rows:
        if row["shares"] > 0:
            quote = lookup(row["quotesymbol"])
            quote["shares"] = 0
            quote["shares"] = row["shares"]
            quote["total"] = round(quote["shares"] * quote["price"], 2)
            portfolio.append(quote)
            balance += quote["total"]
            quote["total"] = usd(quote["total"])
            quote["price"] = usd(quote["price"])

    return render_template("index.html", portfolio=portfolio, cash=usd(round(cash[0]["cash"],2)), balance=usd(balance+cash[0]["cash"]))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        if not request.form.get("symbol"):
            return apology("Must provide symbol", 403)

        if not request.form.get("shares"):
            return apology("Must provide the amount of shares to buy", 403)

        if not request.form.get("shares").isdigit():
            return apology("The amount of shares to buy has to be a positive integer", 403)

        if int(request.form.get("shares")) == 0:
            return apology("The amount of shares to buy cannot be zero", 403)

        # Lookup the quote
        quote = lookup(request.form.get("symbol"))
        if not quote:
            error = "Quote '" + request.form.get("symbol") + "' not found"
            return render_template("buy.html", error=error)

        # Check if the user can afford to buy the stocks
        rows = db.execute("SELECT * FROM users WHERE id = :id", id=session["user_id"])
        cash = rows[0]["cash"]
        total = quote["price"] * int(request.form.get("shares"))

        # Tell the user if there not enough cash in the account
        if cash < total:
            return render_template("buy.html", error="You does not have enough cash in your account")

        # Otherwise record the transaction into database and substract the cost of shares from user cash
        db.execute("INSERT INTO transactions (userid, quotesymbol, unitprice, amount, timestamp) VALUES (:userid, :quotesymbol, :unitprice, :amount, datetime('now'))", userid=session["user_id"], quotesymbol=quote["symbol"], unitprice=quote["price"], amount=int(request.form.get("shares")))
        db.execute("UPDATE users SET cash = :newbalance WHERE id = :id ", newbalance=cash-total, id=session["user_id"])

        # Inform the user that they have successfully bought the shares and redirect them to the homepage
        flash("You have bought " + request.form.get("shares") + " shares of " + quote["name"])
        return redirect("/")
    else:
        return render_template("buy.html")

@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    rows = db.execute("SELECT * FROM transactions WHERE userid=:userid", userid=session["user_id"])

    transactions = []
    for row in rows:
        quote = lookup(row["quotesymbol"])
        transaction = {}
        transaction["symbol"] = row["quotesymbol"]
        transaction["name"] = quote["name"]
        transaction["shares"] = abs(row["amount"])
        transaction["price"] = usd(row["unitprice"])
        if row["amount"] < 0:
            transaction["type"] = "SELL"
        else:
            transaction["type"] = "BUY"
        transaction["date"] = row["timestamp"]
        transaction["total"] = usd(abs(row["amount"]) * row["unitprice"])

        transactions.append(transaction)

    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        # Get the latest data on given quote
        quote = lookup(request.form.get("symbol"))
        if not quote:
            error = "Quote '" + request.form.get("symbol") + "' not found"
            return render_template("quote.html", error=error)
        else:
            return render_template("quoted.html", company=quote["name"], symbol=quote["symbol"], price=usd(quote["price"]))
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # Forget any user id
    session.clear()

    # User reached route via POST
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure username is unique
        # First, query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # then, check if the username is already taken
        if len(rows) > 0:
            return apology("Username already taken", 403)

        # Ensure password was submitted
        if not request.form.get("password"):
            return apology("must provide password", 403)

        # Ensure password was confirmed
        if not request.form.get("confirmation"):
            return apology("must confirm password", 403)

        # Ensure confimration matches the password
        if request.form.get("password") != request.form.get("confirmation"):
            return apology("confirmation must match the password", 403)

        # Hash the password
        pwdhash = generate_password_hash(request.form.get("password"))

        # Add user to database
        db.execute("INSERT INTO users (username, hash) VALUES (:name, :hash)", name=request.form.get("username"), hash=pwdhash)

        # Login user
        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Remember the registered user
        session["user_id"] = rows[0]["id"]

        # Inform the user about being successfully registered
        flash("You have been successfully registered")

        # Redirect user to homepage
        return redirect("/")

    # User reached route via GET
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        if not request.form.get("symbol"):
            return apology("Must provide symbol", 403)

        if not request.form.get("shares"):
            return apology("Must provide the amount of shares to buy", 403)

        if not request.form.get("shares").isdigit():
            return apology("The amount of shares to sell has to be a positive integer", 403)

        if int(request.form.get("shares")) == 0:
            return apology("The amount of shares to sell cannot be zero", 403)

        # Lookup the quote
        quote = lookup(request.form.get("symbol"))
        if not quote:
            error = "Quote '" + request.form.get("symbol") + "' not found"
            return render_template("buy.html", error=error)

        # Check if the user have enough quotes to sell
        rows = db.execute("SELECT SUM(amount) AS shares FROM transactions WHERE userid=:userid AND quotesymbol=:quotesymbol ORDER BY quotesymbol", userid=session["user_id"], quotesymbol=quote["symbol"])

        # Tell the user if they does not have enough stocks to sell
        if int(request.form.get("shares")) > rows[0]["shares"]:
            return render_template("sell.html", error="You do not have enough stocks to sell")

        # Otherwise record the transaction into database and add the profit to user cash
        db.execute("INSERT INTO transactions (userid, quotesymbol, unitprice, amount, timestamp) VALUES (:userid, :quotesymbol, :unitprice, :amount, datetime('now'))", userid=session["user_id"], quotesymbol=quote["symbol"], unitprice=quote["price"], amount=-int(request.form.get("shares")))
        cash = db.execute("SELECT cash FROM users WHERE id=:id", id=session["user_id"])[0]["cash"]
        profit = int(request.form.get("shares")) * quote["price"]
        db.execute("UPDATE users SET cash = :newbalance WHERE id = :id ", newbalance=cash+profit, id=session["user_id"])

        # Inform the user that they have successfully sold their shares and redirect them to the homepage
        flash("You have sold " + request.form.get("shares") + " shares of " + quote["name"])
        return redirect("/")
    else:
        return render_template("sell.html")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


if __name__ == "__main__":
    app.run()

    # Listen for errors
    for code in default_exceptions:
        app.errorhandler(code)(errorhandler)
