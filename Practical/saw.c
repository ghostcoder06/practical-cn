#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h> // Added for time functions

#define TIMEOUT 3
#define MAX_FRAMES 5 // Maximum number of frames to send

typedef enum {
    FRAME_DATA,
    FRAME_ACK
} FrameType;

typedef struct {
    FrameType type;
    int sequence;
    char data;
} Frame;

void sendFrame(Frame frame) {
    printf("Sending frame with sequence number %d\n", frame.sequence);
    // Simulating transmission delay
    sleep(1);
}

Frame receiveFrame() {
    Frame frame;
    sleep(1);
    if (rand() % 2 == 0) {
        frame.type = FRAME_DATA;
        frame.sequence = rand() % 2;
        frame.data = 'A' + rand() % 26;
        printf("Received frame with sequence number %d and data: %c\n", frame.sequence, frame.data);
    } else {
        printf("Frame lost\n");
        frame.type = FRAME_ACK;
    }
    return frame;
}

int main() {
    srand((unsigned int)time(NULL)); // Initialize random seed

    int expectedSequence = 0;
    Frame frameToSend;
    bool ackReceived = true;
    time_t startTime, currentTime;

    int framesSent = 0; // Counter for frames sent

    while (framesSent < MAX_FRAMES) {
        if (ackReceived) {
            frameToSend.type = FRAME_DATA;
            frameToSend.sequence = expectedSequence;
            frameToSend.data = 'A' + rand() % 26;
            sendFrame(frameToSend);
            ackReceived = false;
            startTime = time(NULL); // Start timeout timer
            framesSent++; // Increment frames sent
        }

        Frame receivedFrame = receiveFrame();

        if (receivedFrame.type == FRAME_ACK && receivedFrame.sequence == expectedSequence) {
            ackReceived = true;
            expectedSequence = 1 - expectedSequence; // Toggle sequence number
        }

        currentTime = time(NULL);
        if (difftime(currentTime, startTime) >= TIMEOUT) {
            printf("Timeout occurred, resending frame\n");
            ackReceived = true; // Simulate frame resend
        }

        sleep(1); // Sleep for a short time to avoid busy-waiting
    }

    return 0;
}
