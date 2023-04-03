function setClientCheckboxes() {
  // get elements
  let clientPhoneCheckbox = document.getElementById("clientPhoneCheckbox");
  let clientPhoneElement = document.getElementById("clientPhone");

  // check if elements are null
  if(clientPhoneCheckbox == null) {
    console.log("setClientCheckboxes() clientPhoneCheckbox is null");
    return;
  }else if(clientPhoneElement == null) {
    console.log("setClientCheckboxes() clientPhoneElement is null");
    return;
  }

  // set checkboxes
  if(clientPhoneElement.textContent == "")
    clientPhoneCheckbox.disabled = true;
  else  
    clientPhoneCheckbox.checked = true;
}

function setEventCheckboxes() {
  // get checkboxes elements
  let eventTimeCheckbox = document.getElementById("eventTimeCheckbox");
  let eventDurationCheckbox = document.getElementById("eventDurationCheckbox");
  let eventLocationCheckbox = document.getElementById("eventLocationCheckbox");
  let eventObservationsCheckbox = document.getElementById("eventObservationsCheckbox");
  let eventTotalCostCheckbox = document.getElementById("eventTotalCostCheckbox");

  // get text elements
  let eventTimeElement = document.getElementById("eventTime");
  let eventDurationElement = document.getElementById("eventDuration");
  let eventLocationElement = document.getElementById("eventLocation");
  let eventObservationsElement = document.getElementById("eventObservations");
  let eventTotalCostElement = document.getElementById("eventTotalCost");

  let eventObservationTogglerInput = document.getElementById("eventShowObservations");



  // check elements are null
  if(eventTimeElement == null) {
    console.log("setEventCheckboxes() eventTimeElement is null");
    return;
  }else if(eventDurationElement == null) {
    console.log("setEventCheckboxes() eventDurationElement is null");
    return;
  }else if(eventLocationElement == null) {
    console.log("setEventCheckboxes() eventLocationElement is null");
    return;
  }else if(eventObservationsElement == null) {
    console.log("setEventCheckboxes() eventObservationsElement is null");
    return;
  }else if(eventTotalCostElement == null) {
    console.log("setEventCheckboxes() eventTotalCostElement is null");
    return;
  }else if(eventTimeCheckbox == null) {
    console.log("setEventCheckboxes() eventTimeCheckbox is null");
    return;
  }else if(eventDurationCheckbox == null) {
    console.log("setEventCheckboxes() eventDurationCheckbox is null");
    return;
  }else if(eventLocationCheckbox == null) {
    console.log("setEventCheckboxes() eventLocationCheckbox is null");
    return;
  }else if(eventObservationsCheckbox == null) {
    console.log("setEventCheckboxes() eventObservationsCheckbox is null");
    return;
  }else if(eventTotalCostCheckbox == null) {
    console.log("setEventCheckboxes() eventTotalCostCheckbox is null");
    return;
  }else if(eventObservationTogglerInput == null) {
    console.log("setEventCheckboxes() eventObservationTogglerInput is null");
    return;
  }

  // set checkboxes
  if(eventTimeElement.textContent == "")
    eventTimeCheckbox.disabled = true;
  else   
    eventTimeCheckbox.checked = true; 

  if(eventDurationElement.textContent == "")
    eventDurationCheckbox.disabled = true;
  else  
    eventDurationCheckbox.checked = true;

  if(eventLocationElement.textContent == "")
    eventLocationCheckbox.disabled = true;
  else  
    eventLocationCheckbox.checked = true;

  if(eventObservationsElement.textContent == ""){
    eventObservationsCheckbox.disabled = true;
    eventObservationTogglerInput.disabled = true;
  }else  
    eventObservationsCheckbox.checked = true;

  if(eventTotalCostElement.textContent == "")
    eventTotalCostCheckbox.disabled = true;
  else  
    eventTotalCostCheckbox.checked = true;

}

function setServiceCheckboxes(tempElement) {
  // get parent elements for creating dinamic id for checkboxes
  let serviceProviderCheckboxParent = tempElement.querySelector("#serviceProviderCheckboxParent");
  let serviceTimeCheckboxParent = tempElement.querySelector("#serviceTimeCheckboxParent");
  let serviceDurationCheckboxParent = tempElement.querySelector("#serviceDurationCheckboxParent");
  let serviceCostCheckboxParent = tempElement.querySelector("#serviceCostCheckboxParent");
  let serviceDescriptionCheckboxParent = tempElement.querySelector("#serviceDescriptionCheckboxParent");
  let serviceObservationsCheckboxParent = tempElement.querySelector("#serviceObservationsCheckboxParent");
  let serviceTotalCostCheckboxParent = tempElement.querySelector("#serviceTotalCostCheckboxParent");

  // check if parent elements are null
  if(serviceProviderCheckboxParent == null) {
    console.log("setServiceCheckboxes() serviceProviderCheckboxParent is null");
    return;
  }else if(serviceTimeCheckboxParent == null) {
    console.log("setServiceCheckboxes() serviceTimeCheckboxParent is null");
    return;
  }else if(serviceDurationCheckboxParent == null) {
    console.log("setServiceCheckboxes() serviceDurationCheckboxParent is null");
    return;
  }else if(serviceCostCheckboxParent == null) {
    console.log("setServiceCheckboxes() serviceCostCheckboxParent is null");
    return;
  }else if(serviceDescriptionCheckboxParent == null) {
    console.log("setServiceCheckboxes() serviceDescriptionCheckboxParent is null");
    return;
  }else if(serviceObservationsCheckboxParent == null) {
    console.log("setServiceCheckboxes() serviceObservationsCheckboxParent is null");
    return;
  }else if(serviceTotalCostCheckboxParent == null) {
    console.log("setServiceCheckboxes() serviceTotalCostCheckboxParent is null");
    return;
  }

  // create checkboxes
  let serviceProviderCheckboxInput = document.createElement("input");
  let serviceProviderCheckboxLabel = document.createElement("label");
  serviceProviderCheckboxInput.type = "checkbox";
  serviceProviderCheckboxInput.id = "serviceProviderCheckbox" + tempElement.id;
  serviceProviderCheckboxInput.className = "copy-checkbox";
  serviceProviderCheckboxLabel.className = "copy-label";
  serviceProviderCheckboxLabel.htmlFor = "serviceProviderCheckbox" + tempElement.id;
  serviceProviderCheckboxParent.appendChild(serviceProviderCheckboxInput);
  serviceProviderCheckboxParent.appendChild(serviceProviderCheckboxLabel);

  let serviceTimeCheckboxInput = document.createElement("input");
  let serviceTimeCheckboxLabel = document.createElement("label");
  serviceTimeCheckboxInput.type = "checkbox";
  serviceTimeCheckboxInput.id = "serviceTimeCheckbox" + tempElement.id;
  serviceTimeCheckboxInput.className = "copy-checkbox";
  serviceTimeCheckboxLabel.className = "copy-label";
  serviceTimeCheckboxLabel.htmlFor = "serviceTimeCheckbox" + tempElement.id;

  let serviceDurationCheckboxInput = document.createElement("input");
  let serviceDurationCheckboxLabel = document.createElement("label");
  serviceDurationCheckboxInput.type = "checkbox";
  serviceDurationCheckboxInput.id = "serviceDurationCheckbox" + tempElement.id;
  serviceDurationCheckboxInput.className = "copy-checkbox";
  serviceDurationCheckboxLabel.className = "copy-label";
  serviceDurationCheckboxLabel.htmlFor = "serviceDurationCheckbox" + tempElement.id;

  let serviceCostCheckboxInput = document.createElement("input");
  let serviceCostCheckboxLabel = document.createElement("label");
  serviceCostCheckboxInput.type = "checkbox";
  serviceCostCheckboxInput.id = "serviceCostCheckbox" + tempElement.id;
  serviceCostCheckboxInput.className = "copy-checkbox";
  serviceCostCheckboxLabel.className = "copy-label";
  serviceCostCheckboxLabel.htmlFor = "serviceCostCheckbox" + tempElement.id;

  let serviceDescriptionCheckboxInput = document.createElement("input");
  let serviceDescriptionCheckboxLabel = document.createElement("label");
  serviceDescriptionCheckboxInput.type = "checkbox";
  serviceDescriptionCheckboxInput.id = "serviceDescriptionCheckbox" + tempElement.id;
  serviceDescriptionCheckboxInput.className = "copy-checkbox";
  serviceDescriptionCheckboxLabel.className = "copy-label";
  serviceDescriptionCheckboxLabel.htmlFor = "serviceDescriptionCheckbox" + tempElement.id;

  let serviceObservationsCheckboxInput = document.createElement("input");
  let serviceObservationsCheckboxLabel = document.createElement("label");
  serviceObservationsCheckboxInput.type = "checkbox";
  serviceObservationsCheckboxInput.id = "serviceObservationsCheckbox" + tempElement.id;
  serviceObservationsCheckboxInput.className = "copy-checkbox";
  serviceObservationsCheckboxLabel.className = "copy-label";
  serviceObservationsCheckboxLabel.htmlFor = "serviceObservationsCheckbox" + tempElement.id;

  let serviceTotalCostCheckboxInput = document.createElement("input");
  let serviceTotalCostCheckboxLabel = document.createElement("label");
  serviceTotalCostCheckboxInput.type = "checkbox";
  serviceTotalCostCheckboxInput.id = "serviceTotalCostCheckbox" + tempElement.id;
  serviceTotalCostCheckboxInput.className = "copy-checkbox";
  serviceTotalCostCheckboxLabel.className = "copy-label";
  serviceTotalCostCheckboxLabel.htmlFor = "serviceTotalCostCheckbox" + tempElement.id;

  // get elements to check if they are empty
  let serviceProviderElement = tempElement.querySelector("#serviceProvider");
  let serviceTimeElement = tempElement.querySelector("#serviceTime"); 
  let serviceDurationElement = tempElement.querySelector("#serviceDuration");
  let serviceCostElement = tempElement.querySelector("#serviceCost");
  let serviceTotalCostElement = tempElement.querySelector("#serviceTotalCost");
  let serviceDescriptionElement = tempElement.querySelector("#serviceDescription");
  let serviceObservationsElement = tempElement.querySelector("#serviceObservations");

  let serviceDescriptionTogglerInput = serviceDescriptionElement.parentElement.querySelector(".show-more-input");
  let serviceObservationsTogglerInput = serviceObservationsElement.parentElement.querySelector(".show-more-input");

  let serviceObservationsTogglerLabel = serviceObservationsElement.parentElement.querySelector(".show-more-label");
  let serviceDescriptionTogglerLabel = serviceDescriptionElement.parentElement.querySelector(".show-more-label");

  

  // check if elements are null
  if(serviceProviderElement == null) {
    console.log("setServiceCheckboxes() serviceProviderElement is null");
    return;
  }else if(serviceTimeElement == null) {
    console.log("setServiceCheckboxes() serviceTimeElement is null");
    return;
  }else if(serviceDurationElement == null) {
    console.log("setServiceCheckboxes() serviceDurationElement is null");
    return;
  }else if(serviceCostElement == null) {
    console.log("setServiceCheckboxes() serviceCostElement is null");
    return;
  }else if(serviceDescriptionElement == null) {
    console.log("setServiceCheckboxes() serviceDescriptionElement is null");
    return;
  }else if(serviceObservationsElement == null) {
    console.log("setServiceCheckboxes() serviceObservationsElement is null");
    return;
  }else if(serviceTotalCostElement == null) {
    console.log("setServiceCheckboxes() serviceTotalCostElement is null");
    return;
  }else if(serviceDescriptionTogglerInput == null) {
    console.log("setServiceCheckboxes() serviceDescriptionTogglerInput is null");
    return;
  }else if(serviceObservationsTogglerInput == null) {
    console.log("setServiceCheckboxes() serviceObservationsTogglerInput is null");
    return;
  }else if(serviceDescriptionTogglerLabel == null) {
    console.log("setServiceCheckboxes() serviceDescriptionTogglerLabel is null");
    return;
  }else if(serviceObservationsTogglerLabel == null) {
    console.log("setServiceCheckboxes() serviceObservationsTogglerLabel is null");
    return;
  } 

  // set custom id's for toggler inputs, matching the lables
  serviceDescriptionTogglerInput.id = "serviceDescriptionTogglerInput" + tempElement.id;
  serviceObservationsTogglerInput.id = "serviceObservationsTogglerInput" + tempElement.id;
  serviceDescriptionTogglerLabel.htmlFor = "serviceDescriptionTogglerInput" + tempElement.id;
  serviceObservationsTogglerLabel.htmlFor = "serviceObservationsTogglerInput" + tempElement.id;


  // check if elements are empty
  if(serviceProviderElement.textContent == "")
    serviceProviderCheckboxInput.disabled = true;
  else 
    serviceProviderCheckboxInput.checked = true;

  if(serviceTimeElement.textContent == "")
    serviceTimeCheckboxInput.disabled = true;
  else
    serviceTimeCheckboxInput.checked = true;

  if(serviceDurationElement.textContent == "")
    serviceDurationCheckboxInput.disabled = true;
  else
    serviceDurationCheckboxInput.checked = true;

  if(serviceCostElement.textContent == "")
    serviceCostCheckboxInput.disabled = true;
  else
    serviceCostCheckboxInput.checked = true;

  if(serviceDescriptionElement.textContent == ""){
    serviceDescriptionCheckboxInput.disabled = true;
    serviceDescriptionTogglerInput.disabled = true;
  } else {
    serviceDescriptionCheckboxInput.checked = true;
  }
    
  if(serviceObservationsElement.textContent == ""){
    serviceObservationsCheckboxInput.disabled = true;
    serviceObservationsTogglerInput.disabled = true;
  }else {
    serviceObservationsCheckboxInput.checked = true;
  }

  if(serviceTotalCostElement.textContent == "")
    serviceTotalCostCheckboxInput.disabled = true;
  else
    serviceTotalCostCheckboxInput.checked = true;


  // append checkboxes to parent
  serviceProviderCheckboxParent.appendChild(serviceProviderCheckboxInput);
  serviceProviderCheckboxParent.appendChild(serviceProviderCheckboxLabel);

  serviceTimeCheckboxParent.appendChild(serviceTimeCheckboxInput);
  serviceTimeCheckboxParent.appendChild(serviceTimeCheckboxLabel);

  serviceDurationCheckboxParent.appendChild(serviceDurationCheckboxInput);
  serviceDurationCheckboxParent.appendChild(serviceDurationCheckboxLabel);

  serviceCostCheckboxParent.appendChild(serviceCostCheckboxInput);
  serviceCostCheckboxParent.appendChild(serviceCostCheckboxLabel);

  serviceDescriptionCheckboxParent.appendChild(serviceDescriptionCheckboxInput);
  serviceDescriptionCheckboxParent.appendChild(serviceDescriptionCheckboxLabel);

  serviceObservationsCheckboxParent.appendChild(serviceObservationsCheckboxInput);
  serviceObservationsCheckboxParent.appendChild(serviceObservationsCheckboxLabel);

  serviceTotalCostCheckboxParent.appendChild(serviceTotalCostCheckboxInput);
  serviceTotalCostCheckboxParent.appendChild(serviceTotalCostCheckboxLabel);
} 

function getClientData(clientButton){
  let clientNameElement = clientButton.parentElement.parentElement.querySelector("#clientName");
  let clientPhoneElement = clientButton.parentElement.parentElement.querySelector("#clientPhone");

  let clientPhoneCheckbox = clientButton.parentElement.parentElement.querySelector("#clientPhoneCheckbox");

  // check if elements are null
  if(clientNameElement == null) {
    console.log("getClientData() clientNameElement is null");
    return; 
  }else if(clientPhoneElement == null) {
    console.log("getClientData() clientPhoneElement is null");
    return;
  }else if(clientPhoneCheckbox == null) {
    console.log("getClientData() clientPhoneCheckbox is null");
    return;
  }

  let clipboardText = "Client \n" + "Nume: " + clientNameElement.textContent + "\n";

   // check if checkbox is checked and add to string
   if (clientPhoneCheckbox.checked)
   clipboardText += "Telefon: " + clientPhoneElement.textContent + "\n\n";
  return clipboardText;
}

function getEventData(eventButton){
  let eventDateElement = eventButton.parentElement.parentElement.querySelector("#eventDate");
  let eventTimeElement = eventButton.parentElement.parentElement.querySelector("#eventTime");
  let eventDurationElement = eventButton.parentElement.parentElement.querySelector("#eventDuration");
  let eventLocationElement = eventButton.parentElement.parentElement.querySelector("#eventLocation");
  let eventObservationsElement = eventButton.parentElement.parentElement.querySelector("#eventObservations");
  let eventTotalCostElement = eventButton.parentElement.parentElement.querySelector("#eventTotalCost");
  
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
  let clipboardText = "Eveniment \n" + 
                      "Data: " + eventDateElement.textContent + "\n";

  // check if checkbox is checked and add to string
  if (eventTimeCheckbox.checked && eventTimeElement.textContent != "")
  clipboardText += "Ora: " + eventTimeElement.textContent + "\n";
  if (eventDurationCheckbox.checked && eventDurationElement.textContent != "")
    clipboardText += "Durata: " + eventDurationElement.textContent + "\n";
  if (eventLocationCheckbox.checked && eventLocationElement.textContent != "")
    clipboardText += "Locatie: " + eventLocationElement.textContent + "\n";
  if (eventObservationsCheckbox.checked && eventObservationsElement.textContent != "")
    clipboardText += "Observatii:\n" + eventObservationsElement.textContent + "\n";
  if (eventTotalCostCheckbox.checked && eventTotalCostElement.textContent != "")
    clipboardText += "Cost total: " + eventTotalCostElement.textContent + "\n\n";

    return clipboardText;
}

function getServiceData(serviceButton) {
  let newIcon = document.createElement("i");
  newIcon.className = "bi bi-clipboard-check-fill";
  serviceButton.textContent = "";
  serviceButton.appendChild(newIcon);
  let tempId = serviceButton.parentElement.parentElement.id;
  
  // get Elements
  let serviceProviderElement = serviceButton.parentElement.parentElement.querySelector("#serviceProvider");
  let serviceProviderServiceElement = serviceButton.parentElement.parentElement.querySelector("#serviceProviderService");
  let serviceTimeElement = serviceButton.parentElement.parentElement.querySelector("#serviceTime");
  let serviceDurationElement = serviceButton.parentElement.parentElement.querySelector("#serviceDuration");
  let serviceCostElement = serviceButton.parentElement.parentElement.querySelector("#serviceCost");
  let serviceDescriptionElement = serviceButton.parentElement.parentElement.querySelector("#serviceDescription");
  let serviceObservationsElement = serviceButton.parentElement.parentElement.querySelector("#serviceObservations");

  // get Checkboxes
  let serviceProviderCheckbox = serviceButton.parentElement.parentElement.querySelector("#serviceProviderCheckbox"+ tempId);
  let serviceTimeCheckbox = serviceButton.parentElement.parentElement.querySelector("#serviceTimeCheckbox"+ tempId);
  let serviceDurationCheckbox = serviceButton.parentElement.parentElement.querySelector("#serviceDurationCheckbox"+ tempId);
  let serviceCostCheckbox = serviceButton.parentElement.parentElement.querySelector("#serviceCostCheckbox"+ tempId);
  let serviceDescriptionCheckbox = serviceButton.parentElement.parentElement.querySelector("#serviceDescriptionCheckbox"+ tempId);
  let serviceObservationsCheckbox = serviceButton.parentElement.parentElement.querySelector("#serviceObservationsCheckbox"+ tempId);
  
  // check if elements are null
if(serviceProviderElement == null)
  console.log("serviceProviderElement is null");
if(serviceProviderServiceElement == null)
  console.log("serviceProviderServiceElement is null");
if(serviceTimeElement == null)
  console.log("serviceTimeElement is null");
if(serviceDurationElement == null)
  console.log("serviceDurationElement is null");
if(serviceCostElement == null)
  console.log("serviceCostElement is null");
if(serviceDescriptionElement == null)
  console.log("serviceDescriptionElement is null");
if(serviceObservationsElement == null)
  console.log("serviceObservationsElement is null");
if(serviceProviderCheckbox == null)
  console.log("serviceProviderCheckbox is null");
if(serviceTimeCheckbox == null) 
  console.log("serviceTimeCheckbox is null");
if(serviceDurationCheckbox == null)
  console.log("serviceDurationCheckbox is null");
if(serviceCostCheckbox == null)
  console.log("serviceCostCheckbox is null");
if(serviceDescriptionCheckbox == null)
  console.log("serviceDescriptionCheckbox is null");
if(serviceObservationsCheckbox == null)
  console.log("serviceObservationsCheckbox is null");


  // initialize clipboardText
  let clipboardText = "Serviciu: " + serviceProviderServiceElement.textContent + "\n";
  
  // check if checkbox is checked and add to string
  if (serviceProviderCheckbox.checked && serviceProviderElement.childNodes.length != 0)
    clipboardText += "Furnizor: " + serviceProviderElement.textContent + "\n";
  if (serviceTimeCheckbox.checked && serviceTimeElement.childNodes.length != 0)
    clipboardText += "Ora: " + serviceTimeElement.textContent + "\n";
  if (serviceDurationCheckbox.checked && serviceDurationElement.childNodes.length != 0)  
    clipboardText += "Durata: " + serviceDurationElement.textContent + "\n";
  if (serviceCostCheckbox.checked && serviceCostElement.childNodes.length != 0)
    clipboardText += "Tarif/ora: " + serviceCostElement.textContent + "\n";
  if (serviceDescriptionCheckbox.checked && serviceDescriptionElement.childNodes.length != 0)
    clipboardText += "Descriere: " + serviceDescriptionElement.textContent + "\n";
  if (serviceObservationsCheckbox.checked && serviceObservationsElement.childNodes.length != 0)
    clipboardText += "Observatii: " + serviceObservationsElement.textContent + "\n\n";
    
  return clipboardText;
}

function copyClientDataToClipboard(clientButton) {
  let clipboardText = getClientData(clientButton);
  console.log(clipboardText);

  navigator.clipboard.writeText(clipboardText).then(
    function () {
      console.log("Copy cl V");
    },
    function () {
      console.log("Copy cl X");
    }
  );
}

function copyEventDataToClipboard(eventButton) {
  let clipboardText = getEventData(eventButton)
  console.log(clipboardText);

  // copy to clipboard
  navigator.clipboard.writeText(clipboardText).then(
    function () {
      console.log("Copy ev V");
    },
    function () {
      console.log("Copy ev X");
    }
  );
}

function copyServiceDataToClipboard(serviceButton) {
  let clipboardText = getServiceData(serviceButton);
  console.log(clipboardText);
  
  // copy to clipboard
  navigator.clipboard.writeText(clipboardText).then(
    function () {
      console.log("Copy serv V");
    },
    function () {
      console.log("Copy serv X");
    }
  );
}

function copyAllEventData() {
  let clientCopyBtn = document.getElementById("clientCopyBtn");
  let eventCopyBtn = document.getElementById("eventCopyBtn");
  let servicesBtns = document.querySelectorAll(".serviceCopyBtn");

  if(clientCopyBtn == null)
    console.log("clientCopyBtn is null"); 
  if(eventCopyBtn == null)
    console.log("eventCopyBtn is null");
  if(servicesBtns == null)
    console.log("servicesBtns is null");

  let clipboardText;
  clipboardText += getClientData(clientCopyBtn);
  clipboardText += getEventData(eventCopyBtn);

  for (let i = 0; i < servicesBtns.length; i++) { 
    clipboardText += getServiceData(servicesBtns[i]);
  }
  
  console.log(clipboardText);

  navigator.clipboard.writeText(clipboardText).then(
    function () {
      console.log("Copy all V");
    },
    function () {
      console.log("Copy all X");
    } 
  );
}