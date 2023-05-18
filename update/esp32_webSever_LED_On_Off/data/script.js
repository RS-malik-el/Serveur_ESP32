// Gestion de la mise à jour dynamique de l'état de la lampe lors du clic sur le bouton "Valider"
// Si la valeur est "0", on laisse affiché "on", sinon on affiche "blink" avec la valeur correspondante
$(document).ready(function(){
    $("#Valider").click(function(){
        var valeur = $("#blink_delay").val();
        $.post("blink_time",{
            blink_delay: valeur
        });
        
        var lampStatusElement = document.getElementById("lamp-status");
        var lampStatus = lampStatusElement.innerHTML;
        
        if (valeur === "0") {
            // Laisser "on" affiché
            if (lampStatus !== "off") {
                lampStatusElement.innerHTML = "on";
            }
        } else {
            // Afficher "blink" avec la valeur correspondante
            if (lampStatus !== "off") {
                lampStatusElement.innerHTML = "blink (" + valeur + " ms"+")";
            }
        }
    });
});


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