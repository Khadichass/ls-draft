"use strict"

document.getElementById("convertBtn").addEventListener("click", function() {
    let inputText = document.getElementById("inputText").value;
    let lowercaseText = inputText.charAt(0).toUpperCase() + inputText.slice(1).toLowerCase();

    console.log("Lowercase Text:", lowercaseText);  
});

