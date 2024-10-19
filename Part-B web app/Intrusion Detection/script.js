// For Firebase JS SDK v9.12.1
import { initializeApp } from 'https://www.gstatic.com/firebasejs/9.12.1/firebase-app.js';
import { getDatabase, ref, onValue } from 'https://www.gstatic.com/firebasejs/9.12.1/firebase-database.js';

// Your Firebase configuration
const firebaseConfig = {
    apiKey: "AIzaSyDu-Rm1PnTH4HtLSItFNnVB0wlCYql13II",
    authDomain: "intrusion-detection-20d54.firebaseapp.com",
    databaseURL: "https://intrusion-detection-20d54-default-rtdb.asia-southeast1.firebasedatabase.app",
    projectId: "intrusion-detection-20d54",
    storageBucket: "intrusion-detection-20d54.appspot.com",
    messagingSenderId: "748398125371",
    appId: "1:748398125371:web:cdd8e44febc72379a3571e",
    measurementId: "G-9V07WC7F82"
};

// Initialize Firebase
const app = initializeApp(firebaseConfig);
const database = getDatabase(app);

// DOM element
const statusElement = document.getElementById('status');

// Function to update the status on the webpage
function updateStatus(status) {
    statusElement.textContent = status ? status : "No status available"; // Handle empty status
}

// Read the intrusion status from Firebase
const statusRef = ref(database, '/intrusionStatus');
onValue(statusRef, (snapshot) => {
    if (snapshot.exists()) {
        const status = snapshot.val();
        updateStatus(status);
    } else {
        console.error("No data available in the specified path.");
        updateStatus("No data available"); // Handle no data scenario
    }
}, (error) => {
    console.error("Error fetching data:", error);
    updateStatus("Error fetching status"); // Handle error fetching data
});
