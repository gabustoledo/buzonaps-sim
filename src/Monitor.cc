#include "../includes/Monitor.h"

Monitor::Monitor(string output_dir, bool _debugFlag)
{
    this->outputFile.open(output_dir);
    this->debugFlag = _debugFlag;

    this->logger_thread = thread([this](){
        for(;;)
        {
            this->writeString();
        }
    });
}

void Monitor::writeString()
{
    // Tomamos posesión del mutex
    unique_lock<mutex> lock(this->m);

    // Si la cola está vacía, a mimir nomas
    if (this->cola.empty())
        this->hasElements.wait(lock);

    // Si tiene elementos, los sacamos e imprimimos hasta que no queden más
    while(!this->cola.empty())
    {
        string log = this->cola.front();
        this->outputFile << log << "\n";
        this->cola.pop();
        if (this->debugFlag)
            printf("[LOG] - %s\n", log.c_str());
    }

    // Cuando acaba la cola, dormimos
    this->hasElements.wait(lock);
}

void Monitor::writeLog(json obj)
{
    // Hacemos nuestro el mutex, con pasión y delicadeza
    unique_lock<mutex> lock(this->m);

    // Guardamos el objeto en la cola
    this->cola.push(obj.dump());

    // Avisamos que hay elementos en la cola
    this->hasElements.notify_all();

}

Monitor::~Monitor()
{
    this->outputFile.close();
    // Matamos a la hebra escritora a lo desquiziao no hay tiempos pa giles
    terminate();
}