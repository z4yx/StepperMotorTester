typedef void (*Task_t)(void);

void KeyBoard_Task(void);
void Menu_Task(void);
void Display_Task(void);
void Dial_Task(void);

static const Task_t SystemTasks[] = {
    KeyBoard_Task, 
    Dial_Task, 
    Menu_Task, 
    Display_Task
};

void Task_Run(void)
{
    for(;;){
        for(int i = 0; i < sizeof(SystemTasks)/sizeof(Task_t); i++)
            (SystemTasks[i])();
    }
}