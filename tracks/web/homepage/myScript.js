function myFunction() {
    document.getElementById("demo").innerHTML = "Hello JavaScript";
}

function generateTableHead(table) {
    var i;
    let thead = table.createTHead();
    let row = thead.insertRow()
    let th = document.createElement("th");
    let text = document.createTextNode("test");
    th.appendChild(text);
    row.appendChild(th);
    // for (i = 0; i < 10; i++)
    // {
    // }
}

let table = document.querySelector("table");
generateTableHead(table);
