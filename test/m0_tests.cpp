#include <fcntl.h>
#include <stdio.h>
#include "gtest/gtest.h"
#include <pthread.h>
#include "../include/processing_scheduling.h"
// Using a C library requires extern "C" to prevent function managling
extern "C" {
#include <dyn_array.h>
}


#define NUM_PCB 30
#define QUANTUM 4 // Used for Robin Round for process as the run time limit

unsigned int score;
unsigned int total;

class GradeEnvironment : public testing::Environment {
    public:
        virtual void SetUp() {
            score = 0;
            total = 160;
        }
        virtual void TearDown() {
            ::testing::Test::RecordProperty("points_given", score);
            ::testing::Test::RecordProperty("points_total", total);
            std::cout << "SCORE: " << score << '/' << total << std::endl;
        }
};

/*
*  load_PCB UNIT TEST CASES
**/
TEST(load_process_control_blocks, NullFileName) {
    EXPECT_EQ(NULL, load_process_control_blocks(NULL));
}

TEST(load_process_control_blocks, BadFileName) {
    const char* input_file = "";
    EXPECT_EQ(NULL, load_process_control_blocks(input_file));
}

TEST(load_process_control_blocks, emptyFile) {
    const char* input_file = "empty.bin";
    int fd = open(input_file, O_CREAT);
    close(fd);

    dyn_array_t* pcbs = load_process_control_blocks(input_file);
    EXPECT_EQ(NULL, pcbs);
}
/*
*  first_come_first_serve UNIT TEST CASES
**/
TEST(first_come_first_serve, NullReadyQueue) {
    dyn_array_t* ready_queue = NULL;
    ScheduleResult_t result;
    EXPECT_EQ(false, first_come_first_serve(ready_queue, &result));
}
TEST(first_come_first_serve, NullResult) {
    dyn_array_t* ready_queue = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
    ScheduleResult_t* result = NULL;
    EXPECT_EQ(false, first_come_first_serve(ready_queue, result));
    dyn_array_destroy(ready_queue);
}
TEST(first_come_first_serve, GoodInput) {
    ScheduleResult_t result;
    dyn_array_t * ready_queue = dyn_array_create(0, sizeof(ProcessControlBlock_t), NULL);
    ProcessControlBlock_t b1 = {10, 0, false};
    ProcessControlBlock_t b2 = {15, 2, false};
    dyn_array_push_back(ready_queue, &b1);
    dyn_array_push_back(ready_queue, &b2);

    EXPECT_EQ(true, first_come_first_serve(ready_queue, &result));
    EXPECT_EQ(10.0f / 2, result.average_latency_time);
    dyn_array_destroy(ready_queue);
}

/*
*  priority UNIT TEST CASES
**/
TEST(priority, NullReadyQueue) {
    dyn_array_t* ready_queue = NULL;
    ScheduleResult_t result;
    EXPECT_EQ(false, priority(ready_queue, &result));
}
TEST(priority, NullResult) {
    dyn_array_t* ready_queue = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
    ScheduleResult_t* result = NULL;
    EXPECT_EQ(false, priority(ready_queue, result));
    dyn_array_destroy(ready_queue);
}
TEST(priority, GoodInput) {
    ScheduleResult_t result;
    dyn_array_t * ready_queue = dyn_array_create(0, sizeof(ProcessControlBlock_t), NULL);
    ProcessControlBlock_t b1 = {10, 0, false};
    ProcessControlBlock_t b2 = {15, 2, false};
    dyn_array_push_back(ready_queue, &b1);
    dyn_array_push_back(ready_queue, &b2);

    EXPECT_EQ(true, priority(ready_queue, &result));
    EXPECT_EQ(15.0f / 2, result.average_latency_time);
    dyn_array_destroy(ready_queue);
}

/*
*  round_robin UNIT TEST CASES
**/
TEST(round_robin, NullReadyQueue) {
    dyn_array_t* ready_queue = NULL;
    ScheduleResult_t result;
    EXPECT_EQ(false, round_robin(ready_queue, &result, QUANTUM));
}
TEST(round_robin, NullResult) {
    dyn_array_t* ready_queue = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
    ScheduleResult_t* result = NULL;
    EXPECT_EQ(false, round_robin(ready_queue, result, QUANTUM));
    dyn_array_destroy(ready_queue);
}
TEST(round_robin, ZeroQuantum) {
    dyn_array_t* ready_queue = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
    ScheduleResult_t result;
    size_t quantum = 0;
    EXPECT_EQ(false, round_robin(ready_queue, &result, quantum));
    dyn_array_destroy(ready_queue);
}
TEST(round_robin, GoodInput) {
    ScheduleResult_t result;
    dyn_array_t * ready_queue = dyn_array_create(0, sizeof(ProcessControlBlock_t), NULL);
    ProcessControlBlock_t b1 = {15, 0, false};
    ProcessControlBlock_t b2 = {10, 2, false};
    dyn_array_push_back(ready_queue, &b1);
    dyn_array_push_back(ready_queue, &b2);

    EXPECT_EQ(true, round_robin(ready_queue, &result, QUANTUM));
    EXPECT_EQ(9.0f, result.average_latency_time);
    dyn_array_destroy(ready_queue);
}

/*
*  shortest_job_first UNIT TEST CASES
**/
TEST(shortest_job_first, NullReadyQueue) {
    dyn_array_t* ready_queue = NULL;
    ScheduleResult_t result;
    EXPECT_EQ(false, shortest_job_first(ready_queue, &result));
}
TEST(shortest_job_first, NullResult) {
    dyn_array_t* ready_queue = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
    ScheduleResult_t* result = NULL;
    EXPECT_EQ(false, shortest_job_first(ready_queue, result));
    dyn_array_destroy(ready_queue);
}
TEST(shortest_job_first, GoodInput) {
    ScheduleResult_t result;
    dyn_array_t * ready_queue = dyn_array_create(0, sizeof(ProcessControlBlock_t), NULL);
    ProcessControlBlock_t b1 = {15, 0, false};
    ProcessControlBlock_t b2 = {10, 2, false};
    dyn_array_push_back(ready_queue, &b1);
    dyn_array_push_back(ready_queue, &b2);

    EXPECT_EQ(true, shortest_job_first(ready_queue, &result));
    EXPECT_EQ(10.0f / 2, result.average_latency_time);
    dyn_array_destroy(ready_queue);
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::AddGlobalTestEnvironment(new GradeEnvironment);
    return RUN_ALL_TESTS();

}


