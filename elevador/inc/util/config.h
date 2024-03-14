// Elevator configuration
#define NUM_ELEVATORS 3 //max, not current
#define NUM_FLOORSS 4 //max, not current  // I had double s because it might be used, idk, change when going through the code
#define ELEVATOR_ID "Ole"
#define DRIVER_RETRY_DELAY_MS 1000

// Door configuration
#define DOOR_OPEN_TIME_MS 3000 //ms

// Motor error
#define MOTOR_DRIVING_TIMEOUT_MS 1000 //ms

// Polling rate
#define FSM_RATE_MS 10
#define POLLER_RATE_MS 10 
#define LIGHT_RATE_MS 10

// Network rates
#define BROADCAST_RATE_MS 500
#define CALL_TRANSMIT_RATE_MS 500
#define CALL_RECEIVE_RATE_MS 10
#define STATUS_RECEIVE_RATE_MS 10

// Network broadcast address
#define BROADCAST_ADDRESS "192.168.1.255"

// Network ports
#define BROADCAST_TX_PORT 9879
#define BROADCAST_RX_PORT 12345
#define CALL_TX_PORT 9878
#define CALL_RX_PORT 12346

// Dead connection removal time
#define DEAD_CONNECTION_REMOVAL_TIME_S 5

// Socket clearing
#define SOCKET_CLEAR_RATE_MS 1000

// ACKing
#define ACK_TIMEOUT 1000
#define ACK_RETRIES 10

#define DEBUG 0 // 1 for debug, 0 for no debug --> implement a command line based GUI?