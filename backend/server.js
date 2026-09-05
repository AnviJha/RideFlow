const http = require('http');
const fs = require('fs');
const path = require('path');
const { URL } = require('url');

const PORT = Number(process.env.PORT) || 4173;
const frontendRoot = path.join(__dirname, '..', 'frontend');

const locations = {
  CP: { name: 'Connaught Place', latitude: 28.6315, longitude: 77.2167 },
  KB: { name: 'Karol Bagh', latitude: 28.6519, longitude: 77.1907 },
  HK: { name: 'Hauz Khas', latitude: 28.5494, longitude: 77.2001 },
  SAKET: { name: 'Saket', latitude: 28.5245, longitude: 77.2066 },
  DWARKA: { name: 'Dwarka', latitude: 28.5921, longitude: 77.0460 },
  NOIDA: { name: 'Noida Sector 18', latitude: 28.5708, longitude: 77.3261 }
};

const routeLegs = {
  'CP-SAKET': [52, 41], 'CP-NOIDA': [32, 28], 'CP-KB': [24, 22], 'CP-HK': [30, 27],
  'KB-HK': [28, 25], 'KB-DWARKA': [34, 31], 'HK-SAKET': [22, 20], 'HK-NOIDA': [35, 31],
  'HK-DWARKA': [36, 32], 'SAKET-NOIDA': [38, 34]
};
const vehicleRates = {
  SEDAN: { label: 'Sedan', base: 80, km: 15, min: 2 },
  SUV: { label: 'SUV', base: 120, km: 20, min: 3 },
  BIKE: { label: 'Bike', base: 35, km: 8, min: 1 },
  AUTO: { label: 'Auto', base: 50, km: 10, min: 1.5 }
};
let drivers = [
  { id: 'D1', name: 'Ramesh', vehicle: 'Sedan', type: 'SEDAN', location: 'Karol Bagh', eta: 9, rating: '4.9', initials: 'R', status: 'AVAILABLE' },
  { id: 'D2', name: 'Suresh', vehicle: 'Sedan', type: 'SEDAN', location: 'Hauz Khas', eta: 12, rating: '4.8', initials: 'S', status: 'AVAILABLE' },
  { id: 'D3', name: 'Ganesh', vehicle: 'Sedan', type: 'SEDAN', location: 'Noida Sector 18', eta: 18, rating: '4.9', initials: 'G', status: 'AVAILABLE' },
  { id: 'D4', name: 'Mahesh', vehicle: 'SUV', type: 'SUV', location: 'Dwarka', eta: 21, rating: '4.7', initials: 'M', status: 'AVAILABLE' },
  { id: 'D5', name: 'Vikram', vehicle: 'Bike', type: 'BIKE', location: 'Karol Bagh', eta: 14, rating: '4.8', initials: 'V', status: 'AVAILABLE' },
  { id: 'D6', name: 'Dinesh', vehicle: 'Sedan', type: 'SEDAN', location: 'Hauz Khas', eta: 0, rating: '4.6', initials: 'D', status: 'ON_TRIP' }
];
let rides = [];
let rideCounter = 0;

function json(res, status, payload) {
  res.writeHead(status, { 'Content-Type': 'application/json; charset=utf-8', 'Access-Control-Allow-Origin': '*' });
  res.end(JSON.stringify(payload));
}

function getBody(req) {
  return new Promise((resolve, reject) => {
    let body = '';
    req.on('data', (chunk) => { body += chunk; if (body.length > 1e6) reject(new Error('Request too large')); });
    req.on('end', () => { try { resolve(body ? JSON.parse(body) : {}); } catch { reject(new Error('Invalid JSON')); } });
    req.on('error', reject);
  });
}

function routeFor(source, destination) {
  return routeLegs[`${source}-${destination}`] || routeLegs[`${destination}-${source}`] || [28, 25];
}

function estimate(source, destination, type, pricing) {
  if (!locations[source] || !locations[destination]) throw new Error('Unknown location');
  if (!vehicleRates[type]) throw new Error('Unknown vehicle type');
  const [distanceKm, timeMinutes] = source === destination ? [0, 0] : routeFor(source, destination);
  const rate = vehicleRates[type];
  const multiplier = pricing === 'surge' ? 1.5 : pricing === 'pool' ? 0.75 : 1;
  return { distanceKm, timeMinutes, fare: Number(((rate.base + distanceKm * rate.km + timeMinutes * rate.min) * multiplier).toFixed(2)), vehicle: rate.label, pricing: pricing || 'normal' };
}

function availableDrivers(type) {
  return drivers.filter((driver) => driver.status === 'AVAILABLE' && (!type || driver.type === type));
}

function serveStatic(req, res, pathname) {
  const relativePath = pathname === '/' ? 'index.html' : pathname.replace(/^\/+/, '');
  const filePath = path.resolve(frontendRoot, relativePath);
  if (!filePath.startsWith(path.resolve(frontendRoot))) return json(res, 403, { error: 'Forbidden' });
  fs.readFile(filePath, (error, data) => {
    if (error) return json(res, 404, { error: 'Not found' });
    const extension = path.extname(filePath);
    const types = { '.html': 'text/html; charset=utf-8', '.js': 'text/javascript; charset=utf-8', '.css': 'text/css; charset=utf-8' };
    res.writeHead(200, { 'Content-Type': types[extension] || 'application/octet-stream' });
    res.end(data);
  });
}

const server = http.createServer(async (req, res) => {
  const requestUrl = new URL(req.url, `http://${req.headers.host || 'localhost'}`);
  const { pathname } = requestUrl;
  if (req.method === 'OPTIONS') { res.writeHead(204, { 'Access-Control-Allow-Origin': '*', 'Access-Control-Allow-Methods': 'GET,POST,DELETE,OPTIONS', 'Access-Control-Allow-Headers': 'Content-Type' }); return res.end(); }
  try {
    if (pathname === '/api/health' && req.method === 'GET') return json(res, 200, { ok: true, service: 'ridewise-api' });
    if (pathname === '/api/locations' && req.method === 'GET') return json(res, 200, locations);
    if (pathname === '/api/drivers' && req.method === 'GET') return json(res, 200, availableDrivers(requestUrl.searchParams.get('type') || undefined));
    if (pathname === '/api/rides' && req.method === 'GET') return json(res, 200, rides);
    if (pathname === '/api/estimate' && req.method === 'POST') {
      const body = await getBody(req);
      return json(res, 200, estimate(body.pickup, body.dropoff, body.vehicleType, body.pricing));
    }
    if (pathname === '/api/rides' && req.method === 'POST') {
      const body = await getBody(req);
      if (body.pickup === body.dropoff) return json(res, 400, { error: 'Pickup and drop-off must be different' });
      const trip = estimate(body.pickup, body.dropoff, body.vehicleType, body.pricing);
      const driver = availableDrivers(body.vehicleType)[0] || availableDrivers()[0];
      const ride = { id: `RIDE-${++rideCounter}`, from: locations[body.pickup].name, to: locations[body.dropoff].name, vehicle: trip.vehicle, fare: trip.fare.toFixed(2), distanceKm: trip.distanceKm, timeMinutes: trip.timeMinutes, status: 'DRIVER_ASSIGNED', driver: driver ? driver.name : null, driverEta: driver ? driver.eta : null, date: 'Just now' };
      rides.unshift(ride);
      if (driver) driver.status = 'ON_TRIP';
      return json(res, 201, ride);
    }
    if (pathname === '/api/rides' && req.method === 'DELETE') { rides = []; return json(res, 200, { ok: true }); }
    return serveStatic(req, res, pathname);
  } catch (error) { return json(res, 400, { error: error.message }); }
});

server.listen(PORT, () => console.log(`Ridewise API running at http://localhost:${PORT}`));
