#include <iostream>
using namespace std;

struct Time{
    int hours;
    int minets;
};

Time addTime(Time t1, Time t2){
    Time result; 
    result.minets = t1.minets + t2.minets;
    result.hours = t1.hours + t2.hours + t1.minets / 60;
    result.minets = t1.minets % 60;
    return result;
}

int main(){
    const int DAYS_IN_WEEK = 7;
    Time week[DAYS_IN_WEEK];
    Time total = {0, 0};

    for(int i = 0; i < DAYS_IN_WEEK; i++) {
        cout << "День" << i+1 << ":\n";

        cout << "Время начало работы (Часы и Минуты)";
        cin >> week[i].hours >> week[i].minets;

        cout << "Время завершения работы (часы минуты): ";
        Time end;
        cin >> end.hours >> end.minets;

        Time day_work;
        day_work.minets = end.minets - week[i].minets;
        day_work.hours = end.hours - week[i].hours;

        if(day_work.minets < 0){
            day_work.minets += 60;
            day_work.hours -= 1;
        }

        total = addTime(total, day_work);
    }

        cout << "\nОбщее время работы за неделю: " 
         << total.hours << " часов " 
         << total.minets << " минут" << endl;
    
    return 0;
}