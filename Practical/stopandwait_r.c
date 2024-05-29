#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

#define FRAME_SIZE 100
#define TIMEOUT 5

void receiveFrame(char *frame, int framesize) {
    printf("Received Frame: %s\n", frame);
}

void sendFrame(char *frame, int framesize) {
    printf("Sending Frame: %s\n", frame);
    sleep(1);
}

int main() {
    char frames_to_send[][FRAME_SIZE] = {"frame1", "frame2", "Frame3"};
    int num_frames = sizeof(frames_to_send) / FRAME_SIZE;

    for (int i = 0; i < num_frames; i++) {
        int framesize = strlen(frames_to_send[i]) + 1; // +1 for null terminator
        printf("Sending frame #%d\n", i + 1);
        sendFrame(frames_to_send[i], framesize);

        int timeout_counter = TIMEOUT;
        bool ack_received = false;

        while (!ack_received && timeout_counter > 0) {
            printf("Waiting for ACK...\n");
            sleep(1);
            timeout_counter--;
            // Simulate acknowledgment received randomly
            if (rand() % 2 == 0) {
                ack_received = true;
            }
        }

        if (!ack_received) {
            printf("Timeout! Resending Frame #%d\n", i + 1);
            i--; // Resend the same frame
        } else {
            char received_frame[FRAME_SIZE];
            receiveFrame(received_frame, framesize);
        }
    }

    return 0;
}
