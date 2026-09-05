let locations = {
  CP: { name: 'Connaught Place', distance: 0 },
  KB: { name: 'Karol Bagh', distance: 24 },
  HK: { name: 'Hauz Khas', distance: 30 },
  SAKET: { name: 'Saket', distance: 22 },
  DWARKA: { name: 'Dwarka', distance: 34 },
  NOIDA: { name: 'Noida Sector 18', distance: 32 }
};

const legs = {
  'CP-SAKET': [52, 41], 'CP-NOIDA': [32, 28], 'CP-KB': [24, 22], 'CP-HK': [30, 27],
  'KB-HK': [28, 25], 'KB-DWARKA': [34, 31], 'HK-SAKET': [22, 20], 'HK-NOIDA': [35, 31],
  'HK-DWARKA': [36, 32], 'SAKET-NOIDA': [38, 34]
};
let drivers = [
  { name: 'Ramesh', vehicle: 'Sedan', location: 'Karol Bagh', eta: 9, rating: '4.9', initials: 'R' },
  { name: 'Suresh', vehicle: 'Sedan', location: 'Hauz Khas', eta: 12, rating: '4.8', initials: 'S' },
  { name: 'Ganesh', vehicle: 'Sedan', location: 'Noida Sector 18', eta: 18, rating: '4.9', initials: 'G' },
  { name: 'Mahesh', vehicle: 'SUV', location: 'Dwarka', eta: 21, rating: '4.7', initials: 'M' }
];
const vehicleRates = { SEDAN: { base: 80, km: 15, min: 2 }, SUV: { base: 120, km: 20, min: 3 }, BIKE: { base: 35, km: 8, min: 1 }, AUTO: { base: 50, km: 10, min: 1.5 } };
const placeCoords = { CP: [25, 29], KB: [15, 48], HK: [45, 51], SAKET: [33, 76], DWARKA: [7, 67], NOIDA: [88, 57] };

const pickup = document.querySelector('#pickup');
const dropoff = document.querySelector('#dropoff');
const vehicle = document.querySelector('#vehicle');
const pricing = document.querySelector('#pricing');
const fare = document.querySelector('#fare');
const tripDistance = document.querySelector('#tripDistance');
const tripTime = document.querySelector('#tripTime');
const routeLine = document.querySelector('#routeLine');
const toast = document.querySelector('#toast');
const apiBase = window.location.protocol === 'file:' ? 'http://localhost:4173/api' : '/api';

async function apiRequest(endpoint, options = {}) {
  const response = await fetch(`${apiBase}${endpoint}`, { headers: { 'Content-Type': 'application/json' }, ...options });
  if (!response.ok) throw new Error((await response.json()).error || 'Request failed');
  return response.json();
}

function populateLocations() {
  pickup.replaceChildren();
  dropoff.replaceChildren();
  Object.entries(locations).forEach(([code, place]) => {
    pickup.add(new Option(place.name, code));
    dropoff.add(new Option(place.name, code));
  });
  pickup.value = 'CP';
  dropoff.value = 'SAKET';
}

function getLeg(source, destination) {
  return legs[`${source}-${destination}`] || legs[`${destination}-${source}`] || [28, 25];
}

function updateRoute() {
  const source = pickup.value;
  const destination = dropoff.value;
  const [distance, time] = source === destination ? [0, 0] : getLeg(source, destination);
  const rate = vehicleRates[vehicle.value];
  const multiplier = pricing.value === 'surge' ? 1.5 : pricing.value === 'pool' ? .75 : 1;
  const total = (rate.base + distance * rate.km + time * rate.min) * multiplier;
  fare.textContent = `Rs ${total.toFixed(2)}`;
  tripDistance.textContent = `${distance} km`;
  tripTime.textContent = `${time} min`;
  drawRoute(source, destination);
  apiRequest('/estimate', { method: 'POST', body: JSON.stringify({ pickup: source, dropoff: destination, vehicleType: vehicle.value, pricing: pricing.value }) })
    .then((estimate) => {
      fare.textContent = `Rs ${estimate.fare.toFixed(2)}`;
      tripDistance.textContent = `${estimate.distanceKm} km`;
      tripTime.textContent = `${estimate.timeMinutes} min`;
    }).catch(() => {});
}

function drawRoute(source, destination) {
  document.querySelectorAll('.map-place').forEach((place) => place.classList.toggle('selected', [source, destination].includes(place.dataset.place)));
  if (source === destination) { routeLine.classList.remove('visible'); return; }
  const [x1, y1] = placeCoords[source];
  const [x2, y2] = placeCoords[destination];
  const dx = x2 - x1;
  const dy = y2 - y1;
  routeLine.style.left = `${x1}%`;
  routeLine.style.top = `${y1}%`;
  routeLine.style.width = `${Math.sqrt(dx * dx + dy * dy)}%`;
  routeLine.style.transform = `rotate(${Math.atan2(dy, dx) * 180 / Math.PI}deg)`;
  routeLine.classList.add('visible');
}

function renderDrivers() {
  document.querySelector('.availability').innerHTML = `<i></i> ${drivers.length} available now`;
  document.querySelector('#driverGrid').innerHTML = drivers.map((driver) => `
    <article class="driver-card" data-driver="${driver.id || driver.name}" tabindex="0" role="button" aria-label="View ${driver.name}'s driver details"><div class="driver-top"><span class="driver-avatar">${driver.initials}</span><span class="driver-eta">${driver.eta} min away</span></div><h3>${driver.name}</h3><span class="driver-detail">${driver.vehicle} <span class="footer-dot">&#183;</span> ${driver.location}</span><div class="driver-rating"><span>Rider rating</span><strong>&#9733; ${driver.rating}</strong></div></article>`).join('');
  document.querySelectorAll('.driver-card').forEach((card) => {
    const selectDriver = () => {
      document.querySelectorAll('.driver-card').forEach((item) => item.classList.remove('selected'));
      card.classList.add('selected');
      const driver = drivers.find((item) => String(item.id || item.name) === card.dataset.driver);
      showToast(`${driver.name}: ${driver.vehicle} from ${driver.location}, ${driver.eta} minutes away.`);
    };
    card.addEventListener('click', selectDriver);
    card.addEventListener('keydown', (event) => { if (event.key === 'Enter' || event.key === ' ') { event.preventDefault(); selectDriver(); } });
  });
}

async function renderActivity() {
  let rideHistory;
  try { rideHistory = await apiRequest('/rides'); }
  catch { rideHistory = JSON.parse(localStorage.getItem('ridewise-rides') || '[]'); }
  const list = document.querySelector('#activityList');
  if (!rideHistory.length) { list.innerHTML = '<p class="empty-activity">Your completed rides will appear here.</p>'; return; }
  list.innerHTML = rideHistory.map((ride) => `<article class="activity-item"><span class="activity-icon">&#8599;</span><div><div class="activity-route">${ride.from} <span class="footer-dot">&#183;</span> ${ride.to}</div><div class="activity-meta">${ride.vehicle} <span class="footer-dot">&#183;</span> ${ride.date}</div></div><strong class="activity-price">Rs ${ride.fare}</strong></article>`).join('');
}

function showToast(message) {
  toast.textContent = message;
  toast.classList.add('show');
  window.clearTimeout(showToast.timer);
  showToast.timer = window.setTimeout(() => toast.classList.remove('show'), 3400);
}

document.querySelector('#bookingForm').addEventListener('submit', async (event) => {
  event.preventDefault();
  if (pickup.value === dropoff.value) { showToast('Choose two different locations for your ride.'); return; }
  try {
    const ride = await apiRequest('/rides', { method: 'POST', body: JSON.stringify({ pickup: pickup.value, dropoff: dropoff.value, vehicleType: vehicle.value, pricing: pricing.value }) });
    renderActivity();
    showToast(`Ride requested. ${ride.driver || 'A driver'} is ${ride.driverEta ? `${ride.driverEta} minutes away` : 'being matched'}.`);
  } catch (error) {
    const rides = JSON.parse(localStorage.getItem('ridewise-rides') || '[]');
    rides.unshift({ from: locations[pickup.value].name, to: locations[dropoff.value].name, vehicle: vehicle.options[vehicle.selectedIndex].text.split(' ')[0], fare: fare.textContent.replace('Rs ', ''), date: 'Just now' });
    localStorage.setItem('ridewise-rides', JSON.stringify(rides.slice(0, 5)));
    renderActivity();
    showToast(error.message || 'Unable to request the ride.');
  }
  document.querySelector('#activity').scrollIntoView({ behavior: 'smooth', block: 'center' });
});

[pickup, dropoff, vehicle, pricing].forEach((input) => input.addEventListener('change', updateRoute));
document.querySelectorAll('.map-place').forEach((place) => place.addEventListener('click', () => {
  if (place.dataset.place === pickup.value) dropoff.value = place.dataset.place;
  else pickup.value = place.dataset.place;
  updateRoute();
}));
document.querySelector('#clearActivity').addEventListener('click', async () => {
  try { await apiRequest('/rides', { method: 'DELETE' }); } catch { localStorage.removeItem('ridewise-rides'); }
  renderActivity();
  showToast('Recent activity cleared.');
});

async function loadBackendData() {
  try {
    const [backendLocations, backendDrivers] = await Promise.all([apiRequest('/locations'), apiRequest('/drivers')]);
    locations = backendLocations;
    drivers = backendDrivers;
    populateLocations();
    renderDrivers();
    updateRoute();
  } catch {
    showToast('Offline preview mode: start the backend for live ride data.');
  }
}

populateLocations();
renderDrivers();
renderActivity();
updateRoute();
loadBackendData();
