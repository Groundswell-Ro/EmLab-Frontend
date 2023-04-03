
// function increseTextareaHeightOnFocus(textArea) {
//   let observationsField = textArea.parentElement;
//   let confirmBtn = observationsField.querySelector('.confirmBtn');

//   if(!confirmBtn == null) {
//     console.log('confirmBtn not found');
//     return;
//   }
//   if(!observationsField == null) {
//     console.log('observationsField not found');
//     return;
//   }



//   // add event listener to textarea to increase height on focus
//   textArea.addEventListener('focus', function() {
//     observationsField.style.height = "400px";
//   });
//   // add event listener to textarea to decrease height on losing focus
//   textArea.addEventListener('blur', function() {
//     if(!confirmBtn.activeElement)
//     observationsField.style.height = "100px";
//   });

//   // add event listener to confirm button to decrease height on click
//   confirmBtn.addEventListener('click', function() {
//     observationsField.style.height = "100px";
//   });





// }