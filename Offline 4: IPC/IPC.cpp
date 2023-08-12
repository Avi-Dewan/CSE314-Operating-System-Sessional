// 4 printing semaphore locks, 1 binding(2), 1 submission mutex(read-writing problem)

// student(N) has thread , stuff(2) has thread

//Student: random number as start, leader or not, group, printing Station

// Stuff: 2 read in an intereval


#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include <random>
#include <unistd.h>
#include <chrono>
#include <vector>

using namespace std;


int nStd, grpSize, nGrp, tPrint, tBinding, tRW;


class Student
{
public:
    int id;
    int grp;
    int printStation;
    int delay;
    bool isLeader;
    bool isWaiting;
    sem_t printLock;

    Student() {}

    Student(int studentId, int rdelay) {
        id = studentId;
        grp = (id - 1)/grpSize + 1;
        printStation = (id % 4) + 1;
        isLeader = (id % grpSize == 0);
        delay = rdelay;
        isWaiting = false;
        sem_init(&printLock, 0, 0); // Initialize all printing to 0(locked)

    }

    void printInfo() {
        printf("student: id = %d, grp = %d, printStation = %d, isLeader = %d, delay = %d\n", id, grp, printStation, isLeader, delay);
    }
};

sem_t check_print_stations[4]; // Semaphore locks for 4 printing stations
sem_t binding_station;   // Semaphore lock for binding station
pthread_rwlock_t rw_lock; // rw lock...

vector<pthread_t*> student_threads;
vector<Student*> students;
vector<bool> isPrintMachineEmpty(4, true);
int nSubmissions = 0;


int random_delay(double mean_delay) {
    random_device rd;
    mt19937 gen(rd());
    poisson_distribution<> delay_dist(mean_delay);
    return delay_dist(gen);
}


chrono::steady_clock::time_point start_time;
int getElapsedTime() {
    auto now = chrono::steady_clock::now();
    int elapsed = chrono::duration_cast<chrono::seconds>(now - start_time).count();

    return elapsed;
}




void * thread_student(void * arg)
{
    Student *student = static_cast<Student *>(arg);

    // Random delay before arriving at the printing station
    sleep(student->delay);

    // Trying to access the assigned printing station
    sem_wait(&check_print_stations[student->printStation - 1]);

    if(isPrintMachineEmpty[student->printStation-1]) {
        isPrintMachineEmpty[student->printStation-1] = false;
        sem_post(&student->printLock);
    } 
    else {
        student->isWaiting = true;
    }
    sem_post(&check_print_stations[student->printStation - 1]); // Release the printing station


    printf("Student %d arrived at printing station %d at time %d\n", student->id, student->printStation, getElapsedTime());
    
    if(student->isWaiting)
        printf("Student %d is waiting at printing station %d from time %d\n", student->id, student->printStation, getElapsedTime());

    sleep(1); //random sleep

    sem_wait(&student->printLock);

    printf("Student %d started printing at station %d at time %d\n", student->id, student->printStation, getElapsedTime());

    sleep(tPrint);

    printf("Student %d finished printing at station %d at time %d\n", student->id, student->printStation, getElapsedTime());


    sem_wait(&check_print_stations[student->printStation - 1]);

    student->isWaiting = false;

    bool grpMateFound = false;

    isPrintMachineEmpty[student->printStation] = true;

    //sends message to groupmate

    for(int i = 0; i < nStd; i++) {

        if(student->grp != students[i]->grp) continue;

        if(students[i]->isWaiting && students[i]->printStation == student->printStation ) {
            sem_post(&students[i]->printLock);
            isPrintMachineEmpty[student->printStation] = false;
            grpMateFound = true;
            sleep(1); //random sleep
            printf("Student %d sends completion message to groupmate: %d at time %d\n", student->id, students[i]->id, getElapsedTime());
            break;
        }
    }

    // if groupmate not found, sends message to non-groupmate

    if(!grpMateFound) {
        for(int i = 0; i < nStd; i++) {

          if(student->grp == students[i]->grp) continue;

            if(students[i]->isWaiting && students[i]->printStation == student->printStation) {
                sem_post(&students[i]->printLock);
                isPrintMachineEmpty[student->printStation] = false;
                printf("Student %d sends completion message to non-groupmate: %d at time %d\n", student->id, students[i]->id, getElapsedTime());
                break;
            }
        }
    }


    sem_post(&check_print_stations[student->printStation - 1]); // Release the printing station


    if(!student->isLeader) {
        pthread_exit(NULL);
    }

    //join  other groupmate

    for (int i = student->id - 1; i > student->id - grpSize; i--) {
        pthread_join(*student_threads[i], NULL);
    }

    printf("Group %d has finished printing at time %d\n", student->grp, getElapsedTime());

    
    // Binding phase
    sem_wait(&binding_station);
    sleep(1); //random sleep

    printf("Group %d has started binding  at time %d\n", student->grp, getElapsedTime());
    sleep(tBinding);
    printf("Group %d has finished binding  at time %d\n", student->grp, getElapsedTime());
    sem_post(&binding_station);


    // Submission phase
    pthread_rwlock_wrlock(&rw_lock);
    sleep(tRW);
    printf("Group %d has submitted the report at time %d\n", student->grp, getElapsedTime());
    sleep(2); //random sleep
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
        printf("Staff %d has started reading the entry book at time %d. No. of submission = %d\n", (int*)arg, getElapsedTime(), nSubmissions);
        pthread_rwlock_unlock(&rw_lock);

        if(nSubmissions == nGrp) {
            printf("Staff %d finished reading at %d\n", (int*)arg, getElapsedTime());
            pthread_exit(NULL);
        }
    }
}


int main() {


    // redirecting input and output

    freopen("in.txt", "r", stdin);
    freopen("out.txt", "w", stdout);

    start_time = chrono::steady_clock::now();


    scanf("%d%d", &nStd, &grpSize);
    scanf("%d%d%d", &tPrint, &tBinding, &tRW);

    nGrp= nStd / grpSize;


    // Initialize semaphores and mutex
    for (int i = 0; i < 4; i++) {
        sem_init(&check_print_stations[i], 0, 1); // Initialize all printing stations to 1 (unlocked)
    }

    sem_init(&binding_station, 0, 2); // Initialize binding station to 2 (2 available stations)

    pthread_rwlock_init(&rw_lock, NULL); // Initialize submission book mutex

    
      
    for (int i = 0; i < nStd; i++) {
        int randomDelay = 5 + random_delay(15.0) % 30; // Mean delay of 5 units 
        students.push_back(new Student(i+1, randomDelay));
        student_threads.push_back(new pthread_t());
        pthread_create(student_threads[i], NULL, thread_student, students[i]);
    }

    // Create staff threads
    pthread_t staff_threads[2]; // Assuming 2 staff members as mentioned
    for (int i = 0; i < 2; i++) {
        pthread_create(&staff_threads[i], NULL, thread_stuff, (void *) (i+1));
    }

    // Join group leader threads
    for (int i = 0; i < nStd; i++) {
        if(students[i]->isLeader) pthread_join(*student_threads[i], NULL);
    }

    // Join staff threads
    for (int i = 0; i < 2; i++) {
        pthread_join(staff_threads[i], NULL);
    }

    // Clean up semaphores and mutex
    for (int i = 0; i < 4; i++) {
        sem_destroy(&check_print_stations[i]);
    }
    sem_destroy(&binding_station);
    pthread_rwlock_destroy(&rw_lock);

    return 0;
}
    