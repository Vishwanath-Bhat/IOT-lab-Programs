// Firebase configuration
const firebaseConfig = {
    apiKey: "AIzaSyDGxzJYkftXJTlFamPXHmS6rYL77MUejWg",
    authDomain: "smartparking-6ed51.firebaseapp.com",
    databaseURL: "https://smartparking-6ed51-default-rtdb.asia-southeast1.firebasedatabase.app",
    projectId: "smartparking-6ed51",
    storageBucket: "smartparking-6ed51.appspot.com",
    messagingSenderId: "858175112978",
    appId: "1:858175112978:web:a44089c893379f5bbbd5cb",
    measurementId: "G-ZW2N44GFVQ"
  };
  
  // Initialize Firebase
  firebase.initializeApp(firebaseConfig);
  
  // Reference to the database
  const database = firebase.database();
  
  // Get references to the DOM elements for the parking slots
  const slot1Element = document.getElementById('slot1');
  const slot2Element = document.getElementById('slot2');
  
  // Listen for changes to Parking Slot 1
  database.ref('/parking_spots/spot1').on('value', (snapshot) => {
    const slot1Status = snapshot.val();
    if (slot1Status === 'Occupied') {
      slot1Element.classList.remove('available');
      slot1Element.classList.add('occupied');
      slot1Element.textContent = 'Occupied';
    } else {
      slot1Element.classList.remove('occupied');
      slot1Element.classList.add('available');
      slot1Element.textContent = 'Available';
    }
  });
  
  // Listen for changes to Parking Slot 2
  database.ref('/parking_spots/spot2').on('value', (snapshot) => {
    const slot2Status = snapshot.val();
    if (slot2Status === 'Occupied') {
      slot2Element.classList.remove('available');
      slot2Element.classList.add('occupied');
      slot2Element.textContent = 'Occupied';
    } else {
      slot2Element.classList.remove('occupied');
      slot2Element.classList.add('available');
      slot2Element.textContent = 'Available';
    }
  });
  