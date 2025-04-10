package com.singhastudios.smartrail

import android.os.Bundle
import android.util.Log
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.foundation.Image
import androidx.compose.foundation.background
import androidx.compose.foundation.clickable
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.size
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.material3.Button
import androidx.compose.material3.ButtonDefaults
import androidx.compose.material3.Card
import androidx.compose.material3.CardDefaults
import androidx.compose.material3.DropdownMenu
import androidx.compose.material3.DropdownMenuItem
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.OutlinedButton
import androidx.compose.material3.OutlinedTextField
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Switch
import androidx.compose.material3.Text
import androidx.compose.material3.TopAppBar
import androidx.compose.runtime.Composable
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableDoubleStateOf
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.res.painterResource
import androidx.compose.ui.text.font.FontWeight
import androidx.compose.ui.text.input.PasswordVisualTransformation
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import androidx.navigation.NavController
import androidx.navigation.compose.NavHost
import androidx.navigation.compose.composable
import androidx.navigation.compose.rememberNavController
import com.google.android.gms.maps.model.CameraPosition
import com.google.android.gms.maps.model.LatLng
import com.google.firebase.database.DataSnapshot
import com.google.firebase.database.DatabaseError
import com.google.firebase.database.FirebaseDatabase
import com.google.firebase.database.ValueEventListener
import com.google.maps.android.compose.GoogleMap
import com.google.maps.android.compose.Marker
import com.google.maps.android.compose.MarkerState
import com.google.maps.android.compose.rememberCameraPositionState
import com.singhastudios.smartrail.ui.theme.SmartRailTheme
import kotlin.math.atan2
import kotlin.math.cos
import kotlin.math.sin
import kotlin.math.sqrt

import androidx.compose.foundation.layout.wrapContentSize
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.ArrowDropDown
import androidx.compose.material.icons.filled.Close
import androidx.compose.material.icons.filled.KeyboardArrowUp
import androidx.compose.material.icons.filled.MoreVert
import androidx.compose.material.icons.filled.Warning
import androidx.compose.material3.AlertDialog
import androidx.compose.material3.Icon
import androidx.compose.material3.IconButton
import androidx.compose.ui.graphics.Brush
import androidx.compose.ui.text.style.TextAlign
import com.google.android.gms.maps.CameraUpdate
import com.google.android.gms.maps.model.BitmapDescriptorFactory
import com.google.maps.android.compose.MapProperties
import com.google.maps.android.compose.MapUiSettings
import com.google.maps.android.compose.Polyline

class MainActivity : ComponentActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContent {
            SmartRailTheme {
                AppNavigation()
            }
        }
    }
}

@Composable
fun AppNavigation() {
    val navController = rememberNavController()

    NavHost(navController = navController, startDestination = "splash") {
        composable("splash") { SplashScreen(navController) }
        composable("login") { LoginScreen(navController) }
        composable("main") { MainContent() }
    }
}

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun MainContent() {
    var showEditDialog by remember { mutableStateOf(false) }

    Scaffold(
        modifier = Modifier.fillMaxSize(),
        topBar = {
            TopAppBar(
                title = {
                    Text(
                        text = "SmartRail",
                        fontSize = 18.sp,
                        fontWeight = FontWeight.Bold,
                        color = Color(0xFF1877F2))
                },
                navigationIcon = {
                    Image(
                        painter = painterResource(id = R.drawable.train_main),
                        contentDescription = "Train Icon",
                        modifier = Modifier
                            .size(48.dp)
                            .padding(horizontal = 12.dp)
                    )
                },
                actions = {
                    // Three dots menu icon
                    IconButton(onClick = { showEditDialog = true }) {
                        Icon(Icons.Default.MoreVert, contentDescription = "Menu")
                    }
                }
            )
        }
    ) { innerPadding ->
        Column(modifier = Modifier.padding(innerPadding)) {
            SmartRailUI()
        }

        // Edit station dialog - moved here to be accessible from MainContent
        if (showEditDialog) {
            var stationName by remember { mutableStateOf("") }
            var stationLat by remember { mutableStateOf("") }
            var stationLon by remember { mutableStateOf("") }

            // Firebase database reference
            val database = FirebaseDatabase.getInstance("https://smart-railway-1d2fd-default-rtdb.firebaseio.com/")
            val stationRef = database.getReference("station")

            // Load current station data when dialog opens
            LaunchedEffect(showEditDialog) {
                if (showEditDialog) {
                    stationRef.addListenerForSingleValueEvent(object : ValueEventListener {
                        override fun onDataChange(snapshot: DataSnapshot) {
                            stationName = snapshot.child("name").getValue(String::class.java) ?: ""
                            stationLat = (snapshot.child("latitude").getValue(Double::class.java) ?: 0.0).toString()
                            stationLon = (snapshot.child("longitude").getValue(Double::class.java) ?: 0.0).toString()
                        }

                        override fun onCancelled(error: DatabaseError) {
                            Log.e("Firebase", "Error loading station data: ${error.message}")
                        }
                    })
                }
            }

            AlertDialog(
                onDismissRequest = { showEditDialog = false },
                title = { Text("Edit Station Details") },
                text = {
                    Column {
                        OutlinedTextField(
                            value = stationName,
                            onValueChange = { stationName = it },
                            label = { Text("Station Name") },
                            modifier = Modifier.fillMaxWidth()
                        )

                        OutlinedTextField(
                            value = stationLat,
                            onValueChange = { stationLat = it },
                            label = { Text("Latitude") },
                            modifier = Modifier.fillMaxWidth()
                        )

                        OutlinedTextField(
                            value = stationLon,
                            onValueChange = { stationLon = it },
                            label = { Text("Longitude") },
                            modifier = Modifier.fillMaxWidth()
                        )
                    }
                },
                confirmButton = {
                    Button(
                        onClick = {
                            try {
                                val lat = stationLat.toDouble()
                                val lon = stationLon.toDouble()
                                stationRef.child("name").setValue(stationName)
                                stationRef.child("latitude").setValue(lat)
                                stationRef.child("longitude").setValue(lon)
                                showEditDialog = false
                            } catch (e: NumberFormatException) {
                                // Handle invalid number input
                                // You might want to show a toast or error message here
                            }
                        }
                    ) {
                        Text("Save")
                    }
                },
                dismissButton = {
                    Button(onClick = { showEditDialog = false }) {
                        Text("Cancel")
                    }
                }
            )
        }
    }
}

@Composable
fun SplashScreen(navController: NavController) {
    Box(
        modifier = Modifier
            .fillMaxSize()
            .padding(16.dp),
        contentAlignment = Alignment.Center
    ) {
        Column(
            horizontalAlignment = Alignment.CenterHorizontally
        ) {
            // Train Icon
            Image(
                painter = painterResource(id = R.drawable.train_main),
                contentDescription = "Train Icon",
                modifier = Modifier.size(64.dp),
            )

            // App Title
            Text(
                text = "SmartRail",
                fontSize = 22.sp,
                fontWeight = FontWeight.Bold,
                color = Color.Black,
                modifier = Modifier.padding(top = 8.dp)
            )

            // Tagline
            Text(
                text = "Connecting Journeys, Simplifying Travel",
                fontSize = 14.sp,
                color = Color.Gray,
                modifier = Modifier.padding(top = 4.dp))
        }

        // "Get Started" Button at the bottom center
        Box(
            modifier = Modifier
                .fillMaxSize()
                .padding(bottom = 32.dp),
            contentAlignment = Alignment.BottomCenter
        ) {
            Button(
                onClick = { navController.navigate("login") },
                modifier = Modifier
                    .fillMaxWidth(0.8f)
                    .height(48.dp),
                colors = ButtonDefaults.buttonColors(containerColor = Color(0xFF1E88E5)),
                shape = RoundedCornerShape(24.dp)
            ) {
                Text("Get Started", color = Color.White)
            }

        }
    }
}

@Composable
fun LoginScreen(navController: NavController) {
    Column(
        modifier = Modifier
            .fillMaxSize()
            .padding(16.dp),
        verticalArrangement = Arrangement.Center,
        horizontalAlignment = Alignment.CenterHorizontally
    ) {
        Image(
            painter = painterResource(id = R.drawable.train_main),
            contentDescription = "SmartRail Logo",
            modifier = Modifier.size(48.dp),
        )

        Text(
            text = "SmartRail",
            fontSize = 20.sp,
            fontWeight = FontWeight.Bold,
            color = Color(0xFF1E88E5),
            modifier = Modifier.padding(top = 8.dp, bottom = 16.dp)
        )

        var username by remember { mutableStateOf("") }
        var password by remember { mutableStateOf("") }

        OutlinedTextField(
            value = username,
            onValueChange = { username = it },
            label = { Text("Username") },
            modifier = Modifier.fillMaxWidth()
        )

        Spacer(modifier = Modifier.height(8.dp))

        OutlinedTextField(
            value = password,
            onValueChange = { password = it },
            label = { Text("Password") },
            visualTransformation = PasswordVisualTransformation(),
            modifier = Modifier.fillMaxWidth()
        )

        Spacer(modifier = Modifier.height(16.dp))

        Button(
            onClick = { navController.navigate("main") },
            modifier = Modifier.fillMaxWidth(),
            colors = ButtonDefaults.buttonColors(containerColor = Color(0xFF1E88E5))
        ) {
            Text("Login", color = Color.White)
        }

        Spacer(modifier = Modifier.height(8.dp))

        OutlinedButton(
            onClick = { /* Handle Register */ },
            modifier = Modifier.fillMaxWidth()
        ) {
            Text("Register")
        }

        Spacer(modifier = Modifier.height(16.dp))

        Text(
            text = "Forgot Password?",
            color = Color(0xFF1E88E5),
            fontSize = 14.sp,
            modifier = Modifier.clickable { /* Handle Forgot Password */ }
        )
    }
}

@Composable
fun SmartRailUI() {
    var stationName by remember { mutableStateOf("Loading...") }
    var stationLocation by remember { mutableStateOf(LatLng(0.0, 0.0)) }
    var trainLocation by remember { mutableStateOf(LatLng(0.0, 0.0)) }
    var distance by remember { mutableDoubleStateOf(0.0) }
    var gateControl by remember { mutableStateOf(false) }
    var lightControl by remember { mutableStateOf(false) }
    var buzzerControl by remember { mutableStateOf(false) }

    // Firebase database reference
    val database = FirebaseDatabase.getInstance("https://smart-railway-1d2fd-default-rtdb.firebaseio.com/")
    val stationRef = database.getReference("station")
    val gpsRef = database.getReference("gps")
    val servoRef = database.getReference("servo/servo1")
    val ledRef = database.getReference("led/led1")
    val buzzerRef = database.getReference("buzzer/buzzer1")

    // Fetch station data from Firebase
    LaunchedEffect(Unit) {
        stationRef.addValueEventListener(object : ValueEventListener {
            override fun onDataChange(snapshot: DataSnapshot) {
                val name = snapshot.child("name").getValue(String::class.java) ?: "Unknown Station"
                val lat = snapshot.child("latitude").getValue(Double::class.java) ?: 0.0
                val lon = snapshot.child("longitude").getValue(Double::class.java) ?: 0.0

                stationName = name
                stationLocation = LatLng(lat, lon)

                // Recalculate distance when station updates
                distance = calculateDistance(trainLocation, stationLocation)
            }

            override fun onCancelled(error: DatabaseError) {
                Log.e("Firebase", "Error fetching station: ${error.message}")
            }
        })

        gpsRef.addValueEventListener(object : ValueEventListener {
            override fun onDataChange(snapshot: DataSnapshot) {
                val lat = snapshot.child("latitude").getValue(Double::class.java) ?: 0.0
                val lon = snapshot.child("longitude").getValue(Double::class.java) ?: 0.0

                trainLocation = LatLng(lat, lon)

                // Recalculate distance when train location updates
                distance = calculateDistance(trainLocation, stationLocation)
            }

            override fun onCancelled(error: DatabaseError) {
                Log.e("Firebase", "Error fetching GPS: ${error.message}")
            }
        })

        // Listen to servo state
        servoRef.addValueEventListener(object : ValueEventListener {
            override fun onDataChange(snapshot: DataSnapshot) {
                gateControl = snapshot.getValue(Boolean::class.java) ?: false
            }

            override fun onCancelled(error: DatabaseError) {
                Log.e("Firebase", "Error fetching servo state: ${error.message}")
            }
        })

        // Listen to LED state
        ledRef.addValueEventListener(object : ValueEventListener {
            override fun onDataChange(snapshot: DataSnapshot) {
                lightControl = snapshot.getValue(Boolean::class.java) ?: false
            }

            override fun onCancelled(error: DatabaseError) {
                Log.e("Firebase", "Error fetching LED state: ${error.message}")
            }
        })

        // Listen to buzzer state
        buzzerRef.addValueEventListener(object : ValueEventListener {
            override fun onDataChange(snapshot: DataSnapshot) {
                buzzerControl = snapshot.getValue(Boolean::class.java) ?: false
            }

            override fun onCancelled(error: DatabaseError) {
                Log.e("Firebase", "Error fetching buzzer state: ${error.message}")
            }
        })
    }

    // Handle control switches
    fun onGateControlChanged(newValue: Boolean) {
        gateControl = newValue
        servoRef.setValue(newValue)
    }

    fun onLightControlChanged(newValue: Boolean) {
        lightControl = newValue
        ledRef.setValue(newValue)
    }

    fun onBuzzerControlChanged(newValue: Boolean) {
        buzzerControl = newValue
        buzzerRef.setValue(newValue)
    }

    Column(
        modifier = Modifier.fillMaxSize().padding(16.dp),
        horizontalAlignment = Alignment.CenterHorizontally
    ) {
        Text(text = stationName,
            fontWeight = FontWeight.Bold,
            fontSize = 20.sp)

        // Google Map View
        // In your SmartRailUI composable:
        TrainMapView(
            trainLocation = trainLocation,
            stationLocation = stationLocation
        )

        // Distance Info
        DistanceCard(distance)

        // Control Switches
        GateControlSwitch(gateControl, ::onGateControlChanged)
        LightControlSwitch(lightControl, ::onLightControlChanged)
        BuzzerControlSwitch(buzzerControl, ::onBuzzerControlChanged)
    }
}

@Composable
fun TrainMapView(trainLocation: LatLng, stationLocation: LatLng) {
    val cameraPositionState = rememberCameraPositionState {
        position = CameraPosition.fromLatLngZoom(stationLocation, 10f)
    }

    cameraPositionState.position = CameraPosition.fromLatLngZoom(stationLocation, 10f)


    GoogleMap(
        modifier = Modifier
            .fillMaxWidth()
            .height(200.dp),
        cameraPositionState = cameraPositionState,
        properties = MapProperties(isBuildingEnabled = true),
        uiSettings = MapUiSettings(zoomControlsEnabled = false)
    ) {
        Marker(
            state = MarkerState(position = stationLocation),
            title = "Station",
            icon = BitmapDescriptorFactory.defaultMarker(BitmapDescriptorFactory.HUE_GREEN)
        )
        Marker(
            state = MarkerState(position = trainLocation),
            title = "Train",
            icon = BitmapDescriptorFactory.defaultMarker(BitmapDescriptorFactory.HUE_RED)
        )
    }
}

@Composable
fun DistanceCard(distance: Double) {
    var showWarningDialog by remember { mutableStateOf(false) }

    // Show warning dialog when distance becomes less than 5
    LaunchedEffect(distance) {
        if (distance < 5 && distance > 0) { // >0 to avoid showing on initial load
            showWarningDialog = true
        }
    }

    // Warning Dialog
    if (showWarningDialog) {
        AlertDialog(
            onDismissRequest = { showWarningDialog = false },
            title = {
                Text(
                    text = "Warning!",
                    color = Color.Red,
                    fontWeight = FontWeight.Bold
                )
            },
            text = {
                Text("Train is approaching (${"%.1f".format(distance)} km away)! Please close the gates immediately.")
            },
            confirmButton = {
                Button(
                    onClick = { showWarningDialog = false },
                    colors = ButtonDefaults.buttonColors(containerColor = Color.Red)
                ) {
                    Text("OK", color = Color.White)
                }
            }
        )
    }

    // Card implementation remains the same
    Card(
        modifier = Modifier
            .fillMaxWidth()
            .padding(8.dp),
        elevation = CardDefaults.cardElevation(4.dp),
        shape = RoundedCornerShape(12.dp)
    ) {
        Box(
            modifier = Modifier
                .fillMaxWidth()
                .background(
                    brush = Brush.linearGradient(
                        colors = if (distance < 5) {
                            listOf(Color(0xFFFFEBEE), Color(0xFFFFCDD2)) // Red gradient
                        } else {
                            listOf(Color(0xFFE1F5FE), Color(0xFFB3E5FC)) // Blue gradient
                        }
                    )
                )
                .padding(16.dp)
        ) {
            Column {
                Text(text = "Distance to the station:", fontSize = 16.sp)
                Text(
                    text = "${"%.1f".format(distance)} km",
                    fontSize = 24.sp,
                    fontWeight = FontWeight.Bold,
                    color = if (distance < 5) Color(0xFFD32F2F) else Color(0xFF1877F2)
                )
                if (distance < 5) {
                    Spacer(modifier = Modifier.height(8.dp))
                    Text(
                        text = "Warning: Close the gates!",
                        color = Color(0xFFD32F2F),
                        fontWeight = FontWeight.Bold,
                        fontSize = 14.sp
                    )
                }
            }
        }
    }
}

@Composable
fun GateControlSwitch(gateControl: Boolean, onGateControlChange: (Boolean) -> Unit) {
    Row(
        modifier = Modifier.fillMaxWidth().padding(8.dp),
        verticalAlignment = Alignment.CenterVertically,
        horizontalArrangement = Arrangement.SpaceBetween
    ) {
        Image(
            painter = painterResource(id = R.drawable.gate),
            contentDescription = "Gate Control Icon",
            modifier = Modifier.size(24.dp)
        )
        Text("Gate Control", fontSize = 18.sp)
        Switch(checked = gateControl, onCheckedChange = onGateControlChange)
    }
}

@Composable
fun LightControlSwitch(lightControl: Boolean, onLightControlChange: (Boolean) -> Unit) {
    Row(
        modifier = Modifier.fillMaxWidth().padding(8.dp),
        verticalAlignment = Alignment.CenterVertically,
        horizontalArrangement = Arrangement.SpaceBetween
    ) {
        Image(
            painter = painterResource(id = R.drawable.alert),
            contentDescription = "Light Control Icon",
            modifier = Modifier.size(24.dp)
        )
        Text("Light Control", fontSize = 18.sp)
        Switch(checked = lightControl, onCheckedChange = onLightControlChange)
    }
}

@Composable
fun BuzzerControlSwitch(buzzerControl: Boolean, onBuzzerControlChange: (Boolean) -> Unit) {
    Row(
        modifier = Modifier.fillMaxWidth().padding(8.dp),
        verticalAlignment = Alignment.CenterVertically,
        horizontalArrangement = Arrangement.SpaceBetween
    ) {
        Image(
            painter = painterResource(id = R.drawable.alert), // You might want to use a different icon for buzzer
            contentDescription = "Buzzer Control Icon",
            modifier = Modifier.size(24.dp)
        )
        Text("Buzzer Control", fontSize = 18.sp)
        Switch(checked = buzzerControl, onCheckedChange = onBuzzerControlChange)
    }
}

fun calculateDistance(start: LatLng, end: LatLng): Double {
    val r = 6371 // Radius of Earth in km
    val dLat = Math.toRadians(end.latitude - start.latitude)
    val dLon = Math.toRadians(end.longitude - start.longitude)
    val a = sin(dLat / 2) * sin(dLat / 2) +
            cos(Math.toRadians(start.latitude)) * cos(Math.toRadians(end.latitude)) *
            sin(dLon / 2) * sin(dLon / 2)
    val c = 2 * atan2(sqrt(a), sqrt(1 - a))
    return r * c
}

@Preview(showBackground = true)
@Composable
fun PreviewLoginScreen() {
    LoginScreen(navController = rememberNavController())
}

@Preview(showBackground = true)
@Composable
fun PreviewSplashScreen() {
    SplashScreen(navController = rememberNavController())
}