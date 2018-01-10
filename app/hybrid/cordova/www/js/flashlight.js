/* TODO: Dynamic time slot will be implemented as in Phase2. */
const SLOT = 250;
/* Enable flashlight BUMPER_TIME to carry 8 bits to demonstrate start byte : 0xff */
const BUMPER_TIME = SLOT * 8;
var dataInterval = null;
var bitIndex = 0, byteIndex = 0, firstOccurrence = 1;

function startTransaction(ssid, pass) {
  window.plugins.flashlight.available(function(isAvailable) {
    if (isAvailable) {
      var dataToSent = ssid + '\0' + pass + '\0';

      window.plugins.flashlight.switchOn();

      setTimeout(function() {
        /* Check first bit to decide on/off state in first flash transaction. */
        if(mathConvertionToLight(dataToSent) == 0) {
          window.plugins.flashlight.switchOff();
        }

        dataInterval = setInterval(function() {
          bitIndex++;
          if(mathConvertionToLight(dataToSent) == 1) {
            window.plugins.flashlight.switchOn();
          }
          else {
            window.plugins.flashlight.switchOff();
          }
          checkSteps(dataToSent);
        }, SLOT);

      }, BUMPER_TIME);
    } else {
      alert("Flashlight not available on this device!");
    }
  });
}

function mathConvertionToLight(dataToModulate) {
  return Math.floor(dataToModulate.charCodeAt(byteIndex) / Math.pow(2, 8 - bitIndex - firstOccurrence)) % 2;
}

function checkSteps(dataToModulate) {
  if(bitIndex == 8 - firstOccurrence) {
    firstOccurrence = 0;
    bitIndex = 0;
    byteIndex++;
  }
  if (byteIndex > dataToModulate.length) {
    clearInterval(dataInterval);
    transferCompleted();
    firstOccurrence = 1;
    bitIndex = 0;
    byteIndex = 0;
  }
}
