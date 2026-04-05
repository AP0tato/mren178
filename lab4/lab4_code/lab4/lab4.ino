#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include "grid_graph.h"
     
#define INF 9999 
#define BUZZER_PIN 13   
#define MAX_MSG_LEN 10 

// LCD Keypad Shield Pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// HC-12 Wireless Communication
SoftwareSerial hc12(10, 11);  

#define BTN_RIGHT  0    // ADC value for Right button
#define BTN_UP     99   // ADC value for Up button
#define BTN_DOWN   255  // ADC value for Down button
#define BTN_LEFT   410  // ADC value for Left button
#define BTN_SELECT 640  // ADC value for Select button
#define BTN_NONE   1023 // No button pressed

// Each node in your network needs to have a unique id.  Change NODE_ID
// to be a value between 0 and NUM_NODES-1
const int NODE_ID = 5;  

int destinationNode = 0;
char message[MAX_MSG_LEN + 1];  
int cursorPos = 0;   
char selectedChar = 'A';  

void setup() {
    Serial.begin(9600);
    hc12.begin(9600);
    lcd.begin(16, 2);
    pinMode(BUZZER_PIN, OUTPUT);

    memset(message, ' ', MAX_MSG_LEN);
    message[MAX_MSG_LEN] = '\0'; 

    lcd.print("Enter Msg:");
}

void loop() {
    receiveMessage();  // Always check for incoming messages
    inputMessage();    // Allows for user input message

    if (readButton() == BTN_SELECT) {
        lcd.clear();
        lcd.print("Select Dest:");
        delay(500);
        selectDestination();
        int prev[NUM_NODES];
        findShortestPath(NODE_ID, destinationNode, prev);
        int nextNode = getNextNodeInPath(destinationNode);
        if (nextNode != -1) {
            sendMessage(NODE_ID, nextNode, destinationNode, String(message));
            delay(500);
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Enter Msg:");
        }
    }
}

void receiveMessage() {
    if (hc12.available()) {
        String msg = hc12.readStringUntil('\n'); 
        Serial.println("Received: " + msg);

        if (msg.startsWith("MSG,")) {
            Serial.println("Reading message...");

            int firstComma = msg.indexOf(',', 4);
            int secondComma = msg.indexOf(',', firstComma + 1);
            int thirdComma = msg.indexOf(',', secondComma + 1);
            int messageStart = thirdComma + 1;

            int sender = msg.substring(4, firstComma).toInt();
            int nextNode = msg.substring(firstComma + 1, secondComma).toInt();
            int destination = msg.substring(secondComma + 1, thirdComma).toInt();
            String receivedText = msg.substring(messageStart);

            Serial.println("Sender: "+ String(sender));
            Serial.println("Next Node: " + String(nextNode));
            Serial.println("Destination: " + String(destination));

            // if the message isn't relayed through us, ignore
            if (nextNode != NODE_ID) return;

            if (destination == NODE_ID) {
                lcd.clear();
                lcd.print("Msg from ");
                lcd.print(sender);
                lcd.setCursor(0, 1);
                lcd.print(receivedText);
                playReceiveTone();
                delay(3000);
                lcd.clear();
                lcd.print("Enter Msg:");
            } else {
                int newNextNode = getNextNodeInPath(destination);
                if (newNextNode != -1) {
                    playForwardTone();
                    sendMessage(NODE_ID, newNextNode, destination, receivedText);
                    Serial.println("Message passed to next node in chain.");
                }
            }
        }
    }
}

// Function to get the next node in the shortest path
int getNextNodeInPath(int target) {
    int prev[NUM_NODES];  // Stores previous nodes in the shortest path

    // Call findShortestPath() to populate prev[] array
    findShortestPath(NODE_ID, target, prev);

    // Reconstruct the path to determine the next hop by backtracking through 
    // the prev array
    int node = target;
    while (prev[node] != -1 && prev[node] != NODE_ID) {
        node = prev[node];
    }

    return (prev[node] == NODE_ID) ? node : -1;  // Return next node in path
}

// Function to enter a message using the potentiometer
void inputMessage() {
    receiveMessage();  
    selectedChar = getLetterFromPot();
    int button = readButton();

    if (button == BTN_LEFT) {
        moveCursor(-1);
    } else if (button == BTN_RIGHT) {
        moveCursor(1);
    } else {
        message[cursorPos] = selectedChar;
    }
 
    lcd.setCursor(0, 1);
    lcd.print(message);
    lcd.setCursor(cursorPos, 1);
    lcd.blink();
}

// Function to move cursor left or right
void moveCursor(int direction) {
    int newCursorPos = cursorPos + direction;
    if (newCursorPos >= 0 && newCursorPos < MAX_MSG_LEN) {
        cursorPos = newCursorPos;
    }

    lcd.setCursor(0, 1);
    lcd.print(message);
    lcd.setCursor(cursorPos, 1);
}

// Get the selected letter from the potentiometer
char getLetterFromPot() {
    int potValue = analogRead(A1);
    int index = map(potValue, 0, 1023, 0, 27); // 26 letters + Space 
    return (index < 26) ? ('A' + index) : ' '; // Map 0-25 to A-Z, 26 to space
}

// Function to use up/left/down/right buttons on LCD keypad shield to select 
// destination node
void selectDestination() {
    while (true) {
        receiveMessage();  // Always listen for incoming messages

        lcd.setCursor(0, 1);
        lcd.print("Node ");
        lcd.print(destinationNode);
        lcd.print("    ");

        int button = readButton();

        if (button == BTN_RIGHT || button == BTN_UP) {
            destinationNode = (destinationNode + 1) % NUM_NODES;  // Increment node, wrap around
            delay(200); // Debounce delay
        } else if (button == BTN_LEFT || button == BTN_DOWN) {
            destinationNode = (destinationNode - 1 + NUM_NODES) % NUM_NODES;  // Decrement node, wrap around
            delay(200); // Debounce delay
        } else if (button == BTN_SELECT) {
            lcd.clear();
            lcd.print("Dest Selected:");
            lcd.setCursor(0, 1);
            lcd.print("Node ");
            lcd.print(destinationNode);
            delay(1000);
            return;
        }
    }
}

int readButton() {
    int adc = analogRead(A0);
    delay(100);  // Debounce delay

    if (adc < 50) return BTN_RIGHT;  // Right button pressed
    if (adc < 150) return BTN_UP;     // Up button pressed
    if (adc < 300) return BTN_DOWN;   // Down button pressed
    if (adc < 500) return BTN_LEFT;   // Left button pressed
    if (adc < 800) return BTN_SELECT; // Select button pressed

    return BTN_NONE;  // No button pressed
}

void sendMessage(int source, int nextNode, int destination, String msg) {
    String messagePacket = "MSG," + String(source) + "," + String(nextNode) + "," + String(destination) + "," + msg;
    hc12.println(messagePacket);
    lcd.clear();
    lcd.print("Sent to Node ");
    lcd.setCursor(0, 1);
    lcd.print(nextNode);
    Serial.println("Sent: " + messagePacket);
    playSendTone();
    delay(1000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Enter Msg:");
}

void playSendTone() {
    tone(BUZZER_PIN, 1000, 200);
    delay(250);
    tone(BUZZER_PIN, 1200, 200);
}

void playForwardTone() {
    tone(BUZZER_PIN, 800, 100);  
    delay(150);
    tone(BUZZER_PIN, 1000, 100);
    delay(150);
}

void playReceiveTone() {
    int melody[] = { 262, 330, 392, 523 };
    for (int i = 0; i < 4; i++) {
        tone(BUZZER_PIN, melody[i], 200);
        delay(250);
    }
}

void findShortestPath(int start, int target, int prev[]) {
    int dist[NUM_NODES];
    bool visited[NUM_NODES] = {false};

    // Initialize distances and previous nodes
    for (int i = 0; i < NUM_NODES; i++) {
        dist[i] = INF;
        prev[i] = -1;
    }
    dist[start] = 0;

    // Compute shortest paths using Dijkstra’s Algorithm
    // ------------------------YOUR CODE GOES HERE-----------------------------
    for (int i = 0; i < NUM_NODES; i++) {
        int u = -1;
        int minDistance = INF;
        for (int j = 0; j < NUM_NODES; j++) {
            if (!visited[j] && dist[j] < minDistance) {
                minDistance = dist[j];
                u = j;
            }
        }
        if (u == -1) {
            break;
        }
        visited[u] = true;
        if (u == target) {
            break;
        }
        Edge neighbours[MAX_NEIGHBOURS];
        uint8_t count = graphGetNeighbours((uint8_t)u, neighbours);
        for (uint8_t i = 0; i < count; i++) {
            int v = neighbours[i].to;
            int weight = neighbours[i].weight;

            if (!visited[v]) {
                int newDist = dist[u] + weight;

                if (newDist < dist[v]) {
                    dist[v] = newDist;
                    prev[v] = u;
                }
            }
        }
    }

    // ------------------------------------------------------------------------
    
    // Constructs the path from the prev array
    int path[NUM_NODES];
    int count = 0, node = target;
    while (node != -1) {
        path[count++] = node;
        node = prev[node];
    }

    // Prints the path to screen 
    lcd.clear();
    lcd.print("Path:");
    for (int i = count - 1; i >= 0; i--) {
        lcd.print(path[i]);
        if (i > 0) lcd.print(">");
    }
    delay(2000);
}
