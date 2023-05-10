function buttonOn() {
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "on", true);
    xhttp.send();
    
    // Récupérer l'élément span par son ID
    var lampStatusElement = document.getElementById("lamp-status");
    lampStatusElement.innerHTML = "on";
}

function buttonOff() {
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "off", true);
    xhttp.send();

    // Récupérer l'élément span par son ID
    var lampStatusElement = document.getElementById("lamp-status");
    lampStatusElement.innerHTML = "off";
}