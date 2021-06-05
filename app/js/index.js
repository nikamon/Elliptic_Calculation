const actualBtn = document.getElementById('actual-btn');
let reader = new FileReader();
const fileChosen = document.getElementById('file-chosen');
document.getElementById("y").addEventListener("click", function (event) {
    event.preventDefault();
    return false;
})

actualBtn.addEventListener('change', function () {
    if (actualBtn.files[0] != null) {
        fileChosen.textContent = "Файл успешно выбран";
        document.getElementById("submit").style.display = "block";
    }
})

document.getElementById("submit").addEventListener("click", function () {
    let file = actualBtn.files[0];
    alert(`File name: ${file.name}`); // например, my.png
    reader.readAsText(file);
    reader.onload = function () {
        resetFormData();
        proceedData(reader.result);
    };

});

function resetFormData() {
    document.getElementById('loader').style.display = "none";
    actualBtn.value = "";
    document.getElementById('data-form').reset();
    fileChosen.textContent = "Нажми на меня";
    document.getElementById("submit").style.display = "none";
}

function proceedData(data) {
    generate(data.replace(' ', ''));
    document.getElementById('loader').style.display = "block";

}

function hex2bin(hex) {
    return (parseInt(hex, 16).toString(2)).padStart(8, '0');
}

