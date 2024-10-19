// Firebase configuration
const firebaseConfig = {
    apiKey: "AIzaSyAhipIfTED7gf6ITf74v9Wwbff3K6cOOno",
    authDomain: "weather-monitoring-95ee5.firebaseapp.com",
    databaseURL: "https://weather-monitoring-95ee5-default-rtdb.asia-southeast1.firebasedatabase.app",
    projectId: "weather-monitoring-95ee5",
    storageBucket: "weather-monitoring-95ee5.appspot.com",
    messagingSenderId: "928111415867",
    appId: "1:928111415867:web:60fc529539425ce344821e",
    measurementId: "G-7RLN9MRL69"
  };
  
  // Initialize Firebase
  firebase.initializeApp(firebaseConfig);
  
  // Reference to the database path where the data is stored
  const dbRef = firebase.database().ref('/sensor');
  
  // DOM elements to display the sensor data
  const temperatureElement = document.getElementById('temperature');
  const humidityElement = document.getElementById('humidity');
  const heatIndexElement = document.getElementById('heatIndex');
  
  // Fetch sensor data from Firebase
  dbRef.on('value', (snapshot) => {
    const data = snapshot.val();
    
    // Update the DOM with the new data
    temperatureElement.innerText = data.temperature || '--';
    humidityElement.innerText = data.humidity || '--';
    heatIndexElement.innerText = data.heatIndex || '--';
  }, (error) => {
    console.error('Error fetching data: ', error);
  });
  