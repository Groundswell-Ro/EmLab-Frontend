function setClientCheckboxes() {
  // get Checkboxes
  let clientPhoneCheckbox = document.getElementById("clientPhoneCheckbox");

  // check if checkboxes are null
  if (clientPhoneCheckbox == null) {
    console.log(
      "check the ids of the checkboxes that i get, one of them is null"
    );
    return;
  }

  // set checkboxes to checked
  clientPhoneCheckbox.checked = true;
}

function setEventCheckboxes() {
  // get Checkboxes
  let eventDateCheckbox = document.getElementById("eventDateCheckbox");
  let eventTimeCheckbox = document.getElementById("eventTimeCheckbox");
  let eventDurationCheckbox = document.getElementById("eventDurationCheckbox");
  let eventLocationCheckbox = document.getElementById("eventLocationCheckbox");
  let eventObservationsCheckbox = document.getElementById(
    "eventObservationsCheckbox"
  );
  let eventTotalCostCheckbox = document.getElementById(
    "eventTotalCostCheckbox"
  );

  // check if checkboxes are null
  if (
    eventDateCheckbox == null ||
    eventTimeCheckbox == null ||
    eventDurationCheckbox == null ||
    eventLocationCheckbox == null ||
    eventObservationsCheckbox == null ||
    eventTotalCostCheckbox == null
  ) {
    console.log(
      "check the ids of the checkboxes that i get, one of them is null"
    );
    return;
  }

  // set checkboxes to checked
  eventDateCheckbox.checked = true;
  eventTimeCheckbox.checked = true;
  eventDurationCheckbox.checked = true;
  eventLocationCheckbox.checked = true;
  eventObservationsCheckbox.checked = true;
  eventTotalCostCheckbox.checked = true;
}

function setServicesCheckboxes() {
  let servicesContainer = document.getElementsByClassName("service-copy-container");
  if (servicesContainer == null) {
    console.log("servicesContainer is null");
    return;
  }
  for(let i = 0; i < servicesContainer.length; i++) {
    servicesContainer[i].querySelector("#serviceProviderCheckbox").checked = true;
    servicesContainer[i].querySelector("#serviceProviderServiceCheckbox").checked = true;
    servicesContainer[i].querySelector("#serviceTimeCheckbox").checked = true;
    servicesContainer[i].querySelector("#serviceDurationCheckbox").checked = true;
    servicesContainer[i].querySelector("#serviceCostCheckbox").checked = true;
    servicesContainer[i].querySelector("#serviceDescriptionCheckbox").checked = true;
    servicesContainer[i].querySelector("#serviceObservationsCheckbox").checked = true;
  }
}

function copyClientDataToClipboard(clientButton) {

  let clientNameElement = clientButton.parentElement.parentElement.querySelector("#clientName");
  let clientPhoneElement = clientButton.parentElement.parentElement.querySelector("#clientPhone");

  let clientPhoneCheckbox = clientButton.parentElement.parentElement.querySelector("#clientPhoneCheckbox");

  // check if elements are null
  if (
    clientNameElement == null ||
    clientPhoneElement == null ||
    clientPhoneCheckbox == null
  ) {
    console.log(
      "check the ids of the elements that i get, one of them is null"
    );
    return;
  }

  // initialize strings
  let clipboardText = "Client \n";

  // check if checkbox is checked and add to string
  clipboardText += "Nume: " + clientNameElement.textContent + "\n";
  if (clientPhoneCheckbox.checked)
    clipboardText += "Telefon: " + clientPhoneElement.textContent + "\n";

  console.log(clipboardText);

  navigator.clipboard.writeText(clipboardText).then(
    function () {
      console.log("copy to clipboard successful");
    },
    function () {
      console.log("copy to clipboard failed");
    }
  );
}

function copyEventDataToClipboard(eventButton) {

  let eventDateElement = eventButton.parentElement.parentElement.querySelector("#eventDate");
  let eventTimeElement = eventButton.parentElement.parentElement.querySelector("#eventTime");
  let eventDurationElement = eventButton.parentElement.parentElement.querySelector("#eventDuration");
  let eventLocationElement = eventButton.parentElement.parentElement.querySelector("#eventLocation");
  let eventObservationsElement = eventButton.parentElement.parentElement.querySelector("#eventObservations");
  let eventTotalCostElement = eventButton.parentElement.parentElement.querySelector("#eventTotalCost");
  
  let eventDateCheckbox = eventButton.parentElement.parentElement.querySelector("#eventDateCheckbox");
  let eventTimeCheckbox = eventButton.parentElement.parentElement.querySelector("#eventTimeCheckbox");
  let eventDurationCheckbox = eventButton.parentElement.parentElement.querySelector("#eventDurationCheckbox");
  let eventLocationCheckbox = eventButton.parentElement.parentElement.querySelector("#eventLocationCheckbox");
  let eventObservationsCheckbox = eventButton.parentElement.parentElement.querySelector("#eventObservationsCheckbox");
  let eventTotalCostCheckbox = eventButton.parentElement.parentElement.querySelector("#eventTotalCostCheckbox");

  // check if elements are null
  if (
    eventDateElement == null ||
    eventTimeElement == null ||
    eventDurationElement == null ||
    eventLocationElement == null ||
    eventObservationsElement == null ||
    eventTotalCostElement == null ||
    eventDateCheckbox == null ||
    eventTimeCheckbox == null ||
    eventDurationCheckbox == null ||
    eventLocationCheckbox == null ||
    eventObservationsCheckbox == null ||
    eventTotalCostCheckbox == null
  ) {
    console.log(
      "check the ids of the elements that i get, one of them is null"
    );
    return;
  }
  // initialize clipboardText
  let clipboardText = "Eveniment \n";

  // check if checkbox is checked and add to string
  if (eventDateCheckbox.checked)
    clipboardText += "Data: " + eventDateElement.textContent + "\n";
  if (eventTimeCheckbox.checked)
    clipboardText += "Ora: " + eventTimeElement.textContent + "\n";
  if (eventDurationCheckbox.checked)
    clipboardText += "Durata: " + eventDurationElement.textContent + "\n";
  if (eventLocationCheckbox.checked)
    clipboardText += "Locatie: " + eventLocationElement.textContent + "\n";
  if (eventObservationsCheckbox.checked)
    clipboardText +=
      "Observatii: " + eventObservationsElement.textContent + "\n";
  if (eventTotalCostCheckbox.checked)
    clipboardText += "Cost total: " + eventTotalCostElement.textContent + "\n";

  console.log(clipboardText);

  // copy to clipboard
  navigator.clipboard.writeText(clipboardText).then(
    function () {
      console.log("copy to clipboard successful");
    },
    function () {
      console.log("copy to clipboard failed");
    }
  );
}

function copyServiceDataToClipboard(serviceButton) {
  let newIcon = document.createElement("i");
  newIcon.className = "bi bi-clipboard-check-fill";
  serviceButton.textContent = "";
  serviceButton.appendChild(newIcon);
  
  // get Elements
  let serviceProviderElement = serviceButton.parentElement.parentElement.querySelector("#serviceProvider");
  let serviceProviderServiceElement = serviceButton.parentElement.parentElement.querySelector("#serviceProviderService");
  let serviceTimeElement = serviceButton.parentElement.parentElement.querySelector("#serviceTime");
  let serviceDurationElement = serviceButton.parentElement.parentElement.querySelector("#serviceDuration");
  let serviceCostElement = serviceButton.parentElement.parentElement.querySelector("#serviceCost");
  let serviceDescriptionElement = serviceButton.parentElement.parentElement.querySelector("#serviceDescription");
  let serviceObservationsElement = serviceButton.parentElement.parentElement.querySelector("#serviceObservations");

  // get Checkboxes
  let serviceProviderCheckbox = serviceButton.parentElement.parentElement.querySelector("#serviceProviderCheckbox");
  let serviceProviderServiceCheckbox = serviceButton.parentElement.parentElement.querySelector("#serviceProviderServiceCheckbox");
  let serviceTimeCheckbox = serviceButton.parentElement.parentElement.querySelector("#serviceTimeCheckbox");
  let serviceDurationCheckbox = serviceButton.parentElement.parentElement.querySelector("#serviceDurationCheckbox");
  let serviceCostCheckbox = serviceButton.parentElement.parentElement.querySelector("#serviceCostCheckbox");
  let serviceDescriptionCheckbox = serviceButton.parentElement.parentElement.querySelector("#serviceDescriptionCheckbox");
  let serviceObservationsCheckbox = serviceButton.parentElement.parentElement.querySelector("#serviceObservationsCheckbox");
  
  // check if elements are null
  if (
    serviceProviderElement == null ||
    serviceProviderServiceElement == null ||
    serviceTimeElement == null ||
    serviceDurationElement == null ||
    serviceCostElement == null ||
    serviceDescriptionElement == null ||
    serviceObservationsElement == null ||
    serviceProviderCheckbox == null ||
    serviceProviderServiceCheckbox == null ||
    serviceTimeCheckbox == null ||
    serviceDurationCheckbox == null ||
    serviceCostCheckbox == null ||
    serviceDescriptionCheckbox == null ||
    serviceObservationsCheckbox == null
  ) {
    console.log(
      "check the ids of the elements that i get, one of them is null"
    );
    return;
  }

  // initialize clipboardText
  let clipboardText = "Serviciu \n";
  
  // check if checkbox is checked and add to string
  if (serviceProviderCheckbox.checked)
    clipboardText += "Furnizor: " + serviceProviderElement.textContent + "\n";
  if (serviceProviderServiceCheckbox.checked)
    clipboardText += "Serviciu: " + serviceProviderServiceElement.textContent + "\n";
  if (serviceTimeCheckbox.checked)
    clipboardText += "Ora: " + serviceTimeElement.textContent + "\n";
  if (serviceDurationCheckbox.checked)  
    clipboardText += "Durata: " + serviceDurationElement.textContent + "\n";
  if (serviceCostCheckbox.checked)
    clipboardText += "Cost: " + serviceCostElement.textContent + "\n";
  if (serviceDescriptionCheckbox.checked)
    clipboardText += "Descriere: " + serviceDescriptionElement.textContent + "\n";
  if (serviceObservationsCheckbox.checked)
    clipboardText += "Observatii: " + serviceObservationsElement.textContent + "\n";
    
  console.log(clipboardText);

  // copy to clipboard
  navigator.clipboard.writeText(clipboardText).then(
    function () {
      console.log("copy to clipboard successful");
    },
    function () {
      console.log("copy to clipboard failed");
    }
  );
}

function checkCheckbox(checkboxElement){
  console.log(checkboxElement);
}