#ifndef FIREBASE_H_
#define FIREBASE_H_

#include <stdint.h>
#include "neo6m.h"

// Firebase Realtime Database URL (replace with your own)
#define FIREBASE_URL "https://smart-railway-1d2fd-default-rtdb.firebaseio.com/"

// Firebase Database API key (replace with your own)
#define FIREBASE_API_KEY "AIzaSyD1MasgGuxrfqjEE4_Di8cjR4HuW2eFgwg"

// Function to initialize Firebase
void firebase_init();  // <-- ✅ Add this

// Function to upload GPS coordinates to Firebase
void update_gps_to_firebase(GPS_Coordinates coords);

#endif /* FIREBASE_H_ */
