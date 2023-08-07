// 4 printing semaphore locks, 1 binding(2), 1 submission mutex(read-writing problem)

// student(N) has thread , stuff(2) has thread

//Student: random number as start, leader or not, group, printing Station

// Stuff: 2 read in an intereval


#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include <random>
#include <unistd.h>

using namespace std;


int nStd, grpSize, nGrp, tPrint, tBinding, tRW;

sem_t print_stations[4]; // Semaphore locks for 4 printing stations
sem_t binding_station;   // Semaphore lock for binding station
pthread_rwlock_t rw_lock; // rw lock...



pthread_t student_threads[100];

int nSubmissions = 0;

int random_delay(double mean_delay) {
    random_device rd;
    mt19937 gen(rd());
    exponential_distribution<> delay_dist(1.0 / mean_delay);
    return static_cast<int>(delay_dist(gen));
}


class Student
{
public:
    int id;
    int grp;
    int printStation;
    int delay;
    bool isLeader;

    Student() {}

    Student(int studentId, int rdelay) {
        id = studentId;
        grp = (id - 1)/grpSize + 1;
        printStation = (id % 4) + 1;
        isLeader = (id % grpSize == 0);
        delay = rdelay;
    }

    void printInfo() {
        printf("student: id = %d, grp = %d, printStation = %d, isLeader = %d, delay = %d\n", id, grp, printStation, isLeader, delay);
    }
};




void * thread_student(void * arg)
{
    Student *student = static_cast<Student *>(arg);

    // Random delay before arriving at the printing station
    sleep(student->delay);

    // Trying to access the assigned printing station
    sem_wait(&print_stations[student->printStation - 1]);

    printf("Student %d arrived at printing station %d\n", student->id, student->printStation);

    // Printing phase
    sleep(tPrint);

    // Notifying other students (groupmates)
    // for (int i = 1; i < grpSize; i++) {
    //     int groupmate_id = student->id + i;
    //     if (groupmate_id <= nStd && (groupmate_id % grpSize != 0)) {
    //         printf("Student %d sends a notification to student %d\n", student->id, groupmate_id);
    //     }
    // }

    sem_post(&print_stations[student->printStation - 1]); // Release the printing station

    if(!student->isLeader) {
        pthread_exit(NULL);
    }

    //join your other groupmate

    for (int i = student->id - 1; i > student->id - grpSize; i--) {
        pthread_join(student_threads[i], NULL);
    }

    // Binding phase
    sem_wait(&binding_station);
    printf("Group %d has finished printing %d\n", student->grp, time);
    sleep(tBinding);
    printf("Group %d has finished binding %d\n", student->grp, time);
    sem_post(&binding_station);

    // Submission phase
    pthread_rwlock_wrlock(&rw_lock);
    sleep(tRW);
    printf("Group %d has submitted the report at time %d\n", student->grp, time);
    nSubmissions++;
    pthread_rwlock_unlock(&rw_lock);

    pthread_exit(NULL);
}

void * thread_stuff(void * arg)
{	

    while (true)
    {   
        sleep( 5 + random_delay(5.0));
        pthread_rwlock_rdlock(&rw_lock);
        printf("Staff %d has started reading the entry book at time %d. No. of submission = %d\n", (int*)arg, time, nSubmissions);
        pthread_rwlock_unlock(&rw_lock);

        if(nSubmissions == nGrp) pthread_exit(NULL);

    }
}


int main() {


    // Redirect input from in.txt
    freopen("in.txt", "r", stdin);

    // Redirect output to out.txt
    freopen("out.txt", "w", stdout);


    scanf("%d%d", &nStd, &grpSize);
    scanf("%d%d%d", &tPrint, &tBinding, &tRW);

    nGrp= nStd / grpSize;

    Student students[nStd];

    // Initialize semaphores and mutex
    for (int i = 0; i < 4; i++) {
        sem_init(&print_stations[i], 0, 1); // Initialize all printing stations to 1 (unlocked)
    }

    sem_init(&binding_station, 0, 2); // Initialize binding station to 2 (2 available stations)

    pthread_rwlock_init(&rw_lock, NULL); // Initialize submission book mutex


    
      
    for (int i = 0; i < nStd; i++) {
        int randomDelay = random_delay(5.0); // Mean delay of 5 units 
        students[i] = Student(i+1, randomDelay);
        pthread_create(&student_threads[i], NULL, thread_student, &students[i]);
    }

    // Create staff threads
    pthread_t staff_threads[2]; // Assuming 2 staff members as mentioned
    for (int i = 0; i < 2; i++) {
        pthread_create(&staff_threads[i], NULL, thread_stuff, (void *) (i+1));
    }

    // Join group leader threads
    for (int i = 0; i < nStd; i++) {
        if(students[i].isLeader) pthread_join(student_threads[i], NULL);
    }

    // Join staff threads
    for (int i = 0; i < 2; i++) {
        pthread_join(staff_threads[i], NULL);
    }

    // Clean up semaphores and mutex
    for (int i = 0; i < 4; i++) {
        sem_destroy(&print_stations[i]);
    }
    sem_destroy(&binding_station);
    pthread_rwlock_destroy(&rw_lock);

    return 0;
}
    