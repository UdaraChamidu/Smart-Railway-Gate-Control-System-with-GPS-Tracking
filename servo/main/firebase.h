#ifndef FIREBASE_H_
#define FIREBASE_H_

#include <stdbool.h>

// Firebase Realtime Database URL (replace with your own)
#define FIREBASE_URL "https://smart-railway-1d2fd-default-rtdb.firebaseio.com/"

// Firebase Database API key (replace with your own)
#define FIREBASE_API_KEY "AIzaSyD1MasgGuxrfqjEE4_Di8cjR4HuW2eFgwg"

// Function prototype to read boolean values from Firebase
bool firebase_get_bool(const char *path, bool *out_value);

#endif /* FIREBASE_H_ */
