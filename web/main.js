const SERVICE_UUID = "dedc07fa-e0b0-4aa3-b411-38c95242ebc7"
const CHARACTERISTIC_UUID = "c63bcbc3-af3f-46da-9893-556e6e763f1e"

$("#connect").addEventListener("click", ({ target }) => {
  target.remove();

  start()
})

async function start() {
  const device = await navigator.bluetooth.requestDevice({
    acceptAllDevices: false,
    filters: [{ namePrefix: "xlab" }],
    optionalServices: [SERVICE_UUID]
  })
  const server = await device.gatt.connect();
  const service = await server.getPrimaryService(SERVICE_UUID);
  const characteristic = await service.getCharacteristic(CHARACTERISTIC_UUID);

  $("#value").addEventListener("touchmove", ({ target, targetTouches }) => {
    const touch = targetTouches[0];
    send(characteristic, target, touch);
  });
  $("#value").addEventListener("mousemove", e => {
    send(characteristic, e.target, e);
  });
}

function send(characteristic, target, event) {
  const { clientWidth } = target;
  const { clientX } = event;

  const value = parseInt(255 * (clientX / clientWidth));
  const encoder = new TextEncoder();
  const bleValue = encoder.encode(value);
  characteristic.writeValue(bleValue);
  target.textContent = value;
}

function $(selector) {
  return document.querySelector(selector);
}
