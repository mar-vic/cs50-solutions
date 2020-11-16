function toggleGallery(anchor, albumId) {
    var album = document.querySelector(albumId);

    if (album.style.display === "none") {
        album.style.display = "block";
        anchor.innerHTML = "Hide gallery";
    } else {
        album.style.display = "none";
        anchor.innerHTML = "Show gallery";
    }
}

var berserkAnchor = document.getElementById("toggleberserk");
var tlouAnchor = document.getElementById("toggletlou");
var pixelAnchor = document.getElementById("togglepixel");

berserkAnchor.addEventListener("click", function() { toggleGallery(berserkAnchor, "#berserk"); });
tlouAnchor.addEventListener("click", function() { toggleGallery(tlouAnchor, "#tlou"); });
pixelAnchor.addEventListener("click", function() { toggleGallery(pixelAnchor, "#pixel"); });
