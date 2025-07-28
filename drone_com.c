
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "c_library_v2/common/mavlink.h"  // adjust path as needed

#define BUFFER_LENGTH 2041
#define TARGET_PORT   14550

// Set these for each drone instance
#define SYSTEM_ID     1    // ← Unique per drone
#define COMPONENT_ID  190

void send_heartbeat(int sockfd, struct sockaddr_in *target_addr) {
    mavlink_message_t msg;
    uint8_t buf[BUFFER_LENGTH];

    mavlink_msg_heartbeat_pack(
        SYSTEM_ID, COMPONENT_ID, &msg,
        MAV_TYPE_QUADROTOR, MAV_AUTOPILOT_GENERIC,
        MAV_MODE_GUIDED_ARMED, 0, MAV_STATE_ACTIVE
    );

    uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
    sendto(sockfd, buf, len, 0, (struct sockaddr *)target_addr, sizeof(*target_addr));
}

void send_command_takeoff(int sockfd, struct sockaddr_in *target_addr, float altitude) {
    mavlink_message_t msg;
    uint8_t buf[BUFFER_LENGTH];

    mavlink_msg_command_long_pack(
        SYSTEM_ID, COMPONENT_ID, &msg,
        SYSTEM_ID, COMPONENT_ID,
        MAV_CMD_NAV_TAKEOFF, 0, 0, 0, 0, 0, 0, altitude
    );

    uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
    sendto(sockfd, buf, len, 0, (struct sockaddr *)target_addr, sizeof(*target_addr));
}

void receive_messages(int sockfd) {
    uint8_t buf[BUFFER_LENGTH];
    mavlink_message_t msg;
    mavlink_status_t status;

    struct sockaddr_in src_addr;
    socklen_t addrlen = sizeof(src_addr);
    int bytes = recvfrom(sockfd, buf, BUFFER_LENGTH, 0, (struct sockaddr *)&src_addr, &addrlen);

    for (int i = 0; i < bytes; i++) {
        if (mavlink_parse_char(MAVLINK_COMM_0, buf[i], &msg, &status)) {
            printf("← MSG %d from SYS %d\n", msg.msgid, msg.sysid);

            if (msg.msgid == MAVLINK_MSG_ID_COMMAND_ACK) {
                mavlink_command_ack_t ack;
                mavlink_msg_command_ack_decode(&msg, &ack);
                printf("   ACK: Command %d result %d\n", ack.command, ack.result);
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <target-IP>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *target_ip = argv[1];
    int sockfd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

    struct sockaddr_in target_addr = {0};
    target_addr.sin_family = AF_INET;
    target_addr.sin_port = htons(TARGET_PORT);
    target_addr.sin_addr.s_addr = inet_addr(target_ip);

    printf("Drone SYS_ID %d started. Target: %s:%d\n", SYSTEM_ID, target_ip, TARGET_PORT);

    while (1) {
        send_heartbeat(sockfd, &target_addr);

        // Uncomment if you'd like to send takeoff command periodically
        // send_command_takeoff(sockfd, &target_addr, 10.0);

        receive_messages(sockfd);
        sleep(1);
    }

    close(sockfd);
    return 0;
}

