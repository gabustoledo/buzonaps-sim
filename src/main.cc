#include "../includes/main.h"
#include <json.hpp>
#include <curl/curl.h>
#include <unistd.h>

bool isJsonEmpty(const std::string& jsonStr) {
    try {
        nlohmann::json json = nlohmann::json::parse(jsonStr);
        return json.empty();
    } catch (const std::exception&) {
        return true;
    }
}

size_t writeFunction(void* ptr, size_t size, size_t count, std::string* data) {
    size_t totalSize = size * count;
    data->append(static_cast<char*>(ptr), totalSize);
    return totalSize;
}

int main(int argc, char *argv[]) {

    CURL* curl_config = curl_easy_init();
    std::string url_config = "http://localhost:3000/api/sim/config";

    int opt;
    string config_filepath = "";
    string out_filename = "";
    SimConfig * sim_config = nullptr;
    while((opt = getopt(argc, argv, "f:o:")) != -1) {
        switch(opt) {
            case 'f':
                config_filepath = optarg;
                // sim_config = SimConfig::getInstance(config_filepath);
                break;
            case 'o':
                out_filename = optarg;
                break;
            case 'h':
                printf("HELP\n");
                break;
            default :
                printf("DEMO\n");
                break;
        }
    }

    // realizar get y actualizar config
    url_config = "http://localhost:3000/api/sim/config";
    curl_config = curl_easy_init();
    std::string responseBuffer_config;
    CURLcode res_config;

    if (curl_config) {
        // Realiza la solicitud GET
        curl_easy_setopt(curl_config, CURLOPT_URL, url_config.c_str());

        curl_easy_setopt(curl_config, CURLOPT_WRITEFUNCTION, writeFunction);
        curl_easy_setopt(curl_config, CURLOPT_WRITEDATA, &responseBuffer_config);

        res_config = curl_easy_perform(curl_config);

        // Verifica si hubo errores
        if (res_config != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res_config));
        } else {
            sim_config = SimConfig::getInstanceNew(responseBuffer_config);
        }
        

        // Limpia y libera los recursos de cURL
        curl_easy_cleanup(curl_config);
    }

    Stats * stats = Stats::getInstance();

    printf("INICIANDO SIMULACION\n");
    double END_SIM = sim_config->getParams()["end_sim"].get<double>();
    int cesfam_amount = sim_config->getParams()["cesfam_amount"].get<int>();
    int managers_amount = sim_config->getParams()["managers_amount"].get<int>();
    int patients_amount = sim_config->getParams()["patients_amount"].get<int>();

    EventList * event_list = new EventList(nullptr, 0.0, nullptr);

    auto t = time(nullptr);
    auto tm = *localtime(&t);
    ostringstream time;
    time << put_time(&tm, "%d%m%Y_%H-%M-%S");

    std::string archivo_salida = "./out/out_config_a_1_" + time.str() + ".dat";
    std::string archivo_salida_general = "./out/general_" + time.str() + ".dat";

    System * sys = new System(event_list);
    sys->initializeSystem(cesfam_amount, managers_amount, patients_amount, out_filename, time.str());

    Agent * agent;

    Event * e = new Event();

    int current_day = 0;
    CURL* curl = curl_easy_init();
    std::string url = "http://localhost:3000/api/sim/state";
    double last_clock;

    while(!event_list->isEmpty() && event_list->getClock() <= END_SIM) {
        last_clock = event_list->getClock();

        // printf("last_clock %f\n", last_clock);

        // Se comprueba cuando termina el dia
        int day = last_clock/24;
        if (current_day != day){
            if (day != 0){
                current_day = day;
                printf("El dia a cambiado %d\n", current_day);

                // se obtiene el estado actual del simulador
                std::string state_day = "";
                string lineaLocalSateRead;
                bool primeralinea = true;
                bool archivoVacio = true;
                ifstream fileLocalSateRead(archivo_salida);
                while(getline(fileLocalSateRead, lineaLocalSateRead)){
                    archivoVacio = false;
                    if (!primeralinea) {
                        state_day += lineaLocalSateRead;
                        state_day += "\n";
                    }
                    primeralinea = false;
                }

                // std::cout << state_day << std::endl;

                // se envia como json en un archivo
                if (archivoVacio){
                    state_day = "{\"vacio\":\"vaciosimulador\"}\n";
                }
                std::string clockStr = std::to_string(last_clock);
                state_day += "{\"clock\":\"" + clockStr + "\"}\n";
                // cout << state_day << endl;

                // realiza json y conexion post---------------------

                std::vector<std::string> jsonStrings;
                size_t startPos = 0;
                size_t endPos;

                while ((endPos = state_day.find('\n', startPos)) != std::string::npos) {
                    std::string jsonString = state_day.substr(startPos, endPos - startPos);
                    jsonStrings.push_back(jsonString);
                    startPos = endPos + 1;
                }

                json jsonArray = json::array();

                for (const std::string& jsonString : jsonStrings) {
                    json jsonObj = json::parse(jsonString);

                    json filteredJson;

                    vector<string> attributesToKeep = {"agent_type", "clinical_risk", "manager_id", "patient_id", "sim_clock", "process", "vacio", "clock"};

                    // Iterar sobre los atributos que deseas conservar
                    for (const auto& attr : attributesToKeep) {
                        if (jsonObj.find(attr) != jsonObj.end()) {
                            // Añadir el atributo al nuevo objeto JSON
                            filteredJson[attr] = jsonObj[attr];
                        }
                    }

                    jsonArray.push_back(filteredJson);
                }
                std::string jsonStr = jsonArray.dump();

                // Calcular el tamaño en bytes
                size_t tamaño_bytes = jsonStr.size();

                // Convertir bytes a megabytes
                double tamaño_mb = static_cast<double>(tamaño_bytes) / (1024 * 1024);

                std::cout << "El tamaño del JSON es: " << tamaño_mb << " MB" << std::endl;

                // std::cout << jsonStr << std::endl;

                url = "http://localhost:3000/api/sim/state";
                curl = curl_easy_init();
                printf("Post de estado\n");
                if (curl) {
                    // Establece la URL de destino
                    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

                    // Establece el método de solicitud como POST
                    curl_easy_setopt(curl, CURLOPT_POST, 1L);

                    // Establece los datos JSON en el cuerpo de la solicitud
                    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonStr.c_str());

                    // Realiza la solicitud POST
                    curl_easy_perform(curl);

                    // Limpia y libera los recursos de cURL
                    curl_easy_cleanup(curl);
                }else{
                    printf("problemsa con el post del state\n");
                }

                // -------------------------------------------------

                // se vacia el archivo
                ofstream fileLocalSateWrite(archivo_salida, std::ios::trunc);

                // consulta get para las tareas ------------------------

                url = "http://localhost:3000/api/rl/task";
                curl = curl_easy_init();
                json jsonObjTask;
                printf("Get de tareas\n");
                if (curl) {
                    bool emptyResponse = true;

                    while (emptyResponse) {
                        // Realiza la solicitud GET
                        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

                        std::string responseBuffer;
                        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
                        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBuffer);

                        CURLcode res = curl_easy_perform(curl);

                        // Verifica el resultado de la solicitud
                        if (res == CURLE_OK) {
                            if (!isJsonEmpty(responseBuffer)) {
                                // responseBuffer = responseBuffer.substr(2, responseBuffer.length() - 7);

                                // size_t found = responseBuffer.find("\\\"");
                                // while (found != std::string::npos) {
                                //     responseBuffer.replace(found, 2, "\"");
                                //     found = responseBuffer.find("\\\"", found + 1);
                                // }
                                // // std::cout << "Respuesta no vacía:\n" << std::endl; 
                                emptyResponse = false;

                                jsonObjTask = json::parse(responseBuffer);
                                std::string jsonStrTask = jsonObjTask.dump();
                                // std::cout << jsonStrTask << std::endl;

                            }
                        }
                    }

                    // Limpia y libera los recursos de cURL
                    curl_easy_cleanup(curl);
                }

                // Aqui se deberian agregar las tareas a la lista de eventos
                // Recorrer cada elemento del arreglo JSON
                for (const auto& element : jsonObjTask) {
                    // Obtener cada valor usando su clave
                    double clock_init = element.at("clock_init").get<double>();
                    int id_manager = element.at("id_manager").get<int>();
                    int id_patient = element.at("id_patient").get<int>();
                    string process = element.at("process").get<string>();
                    double execute_time = element.at("execute_time").get<double>();

                    if (!(id_manager == -1)){

                        Patient *patient_task = sys->get_patient_by_id(id_patient);
                        Manager *manager_task = sys->get_manager_by_id(id_manager);

                        Event * event_task;
                        // Dependiendo de cual sea el process, se debe hacer diferente evento
                        if (process == "ASK_CONSENT") {
                            event_task = new Event(CallerType::AGENT_MANAGER, id_manager, clock_init, execute_time, 
                                        ManagerEvents::ASK_CONSENT, patient_task, manager_task, nullptr);
                        }
                        
                        else if (process == "PRE_CLASSIFY_CLINICAL_RISK") {
                            event_task = new Event(CallerType::AGENT_PATIENT, id_manager, clock_init, execute_time, 
                                        ManagerEvents::PRE_CLASSIFY_CLINICAL_RISK, patient_task, manager_task, nullptr);
                        }
                        
                        else if (process == "PRE_CLASSIFY_SOCIAL_RISK") {
                            event_task = new Event(CallerType::AGENT_PATIENT, id_manager, clock_init, execute_time, 
                                        ManagerEvents::PRE_CLASSIFY_SOCIAL_RISK, patient_task, manager_task, nullptr);
                        }
                        
                        else if (process == "MANAGE_PATIENT") {
                            event_task = new Event(CallerType::AGENT_PATIENT, id_manager, clock_init, execute_time, 
                                        ManagerEvents::MANAGE_PATIENT, patient_task, manager_task, nullptr);
                        }
                        
                        else if (process == "MANAGE_MEDICAL_HOUR") {
                            event_task = new Event(CallerType::AGENT_PATIENT, id_manager, clock_init, execute_time, 
                                        ManagerEvents::MANAGE_MEDICAL_HOUR, patient_task, manager_task, nullptr);
                        }
                        
                        else if (process == "MANAGE_TEST_HOUR") {
                            event_task = new Event(CallerType::AGENT_PATIENT, id_manager, clock_init, execute_time, 
                                        ManagerEvents::MANAGE_TEST_HOUR, patient_task, manager_task, nullptr);
                        }
                        
                        else if (process == "MANAGE_SOCIAL_HOUR") {
                            event_task = new Event(CallerType::AGENT_PATIENT, id_manager, clock_init, execute_time, 
                                        ManagerEvents::MANAGE_SOCIAL_HOUR, patient_task, manager_task, nullptr);
                        }
                        
                        else if (process == "MANAGE_PSYCHO_HOUR") {
                            event_task = new Event(CallerType::AGENT_PATIENT, id_manager, clock_init, execute_time, 
                                        ManagerEvents::MANAGE_PSYCHO_HOUR, patient_task, manager_task, nullptr);
                        }
                        
                        else if (process == "RE_EVALUATE_LOW_RISK") {
                            event_task = new Event(CallerType::AGENT_PATIENT, id_manager, clock_init, execute_time, 
                                        ManagerEvents::RE_EVALUATE_LOW_RISK, patient_task, manager_task, nullptr);
                        }
                        
                        else if (process == "RE_EVALUATE_MANAGED") {
                            event_task = new Event(CallerType::AGENT_PATIENT, id_manager, clock_init, execute_time, 
                                        ManagerEvents::RE_EVALUATE_MANAGED, patient_task, manager_task, nullptr);
                        }else{
                            printf("no se encontro nadiiii\n\n");
                        }

                        event_list->insertEvent(event_task);
                    }
                }

                // ------------------------------------------------------
            }
        }

        e = event_list->nextEvent();
        assert(e != nullptr);
        if(e->getCallerType() != CallerType::SYSTEM && e->getCallerPtr() != nullptr)
        {
            agent = e->getObjectivePtr();
            assert(last_clock <= event_list->getClock());
            // printf("----------------------------------------------------------------------------------------------------\n");
            // printf("[ID EVENT %d] - [CLOCK %lf]\n", e->getId(), event_list->getClock());
            agent->processEvent(e);
            continue; 
        }
        sys->processEvent(e);
    }

    if (event_list->isEmpty()){
        printf("no hay mas eventos\n");
    }

    if (event_list->getClock() > END_SIM){
        printf("se llego al finl del clock\n");
        std::cout << event_list->getClock() << std::endl;
    }

    stats->writeOutput("./out/general", time.str());
    ofstream fileLocalSateWrite(archivo_salida, std::ios::trunc);
    fileLocalSateWrite << "vacio";

    printf("TOTAL EVENTS = %d \n", event_list->getLastEvent()->getId());

    // std::cout << last_clock << std::endl;
    std::string clockFinal = std::to_string(last_clock);
    std::string stateFinal = "{\"clock\":\"final\"}\n";
    stateFinal += "{\"clock\":\"" + clockFinal + "\"}\n";
    stateFinal += "{\"clock\":\"" + clockFinal + "\"}\n";

    std::vector<std::string> jsonStrings;
    size_t startPos = 0;
    size_t endPos;

    while ((endPos = stateFinal.find('\n', startPos)) != std::string::npos) {
        std::string jsonString = stateFinal.substr(startPos, endPos - startPos);
        jsonStrings.push_back(jsonString);
        startPos = endPos + 1;
    }

    json jsonArray = json::array();

    for (const std::string& jsonString : jsonStrings) {
        json jsonObj = json::parse(jsonString);
        jsonArray.push_back(jsonObj);
    }
    std::string jsonStr = jsonArray.dump();

    // std::cout << jsonStr << std::endl;

    url = "http://localhost:3000/api/sim/state";
    curl = curl_easy_init();
    if (curl) {
        // Establece la URL de destino
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        // Establece el método de solicitud como POST
        curl_easy_setopt(curl, CURLOPT_POST, 1L);

        // Establece los datos JSON en el cuerpo de la solicitud
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonStr.c_str());

        // Realiza la solicitud POST
        curl_easy_perform(curl);

        // Limpia y libera los recursos de cURL
        curl_easy_cleanup(curl);
    }

    sleep(5);

    // Lectura del archivo final
    std::ifstream file(archivo_salida_general);
    if (!file) {
        std::cerr << "No se pudo abrir el archivo: " << archivo_salida_general << std::endl;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string jsonData = buffer.str();

    url = "http://localhost:3000/api/sim/state";
    curl = curl_easy_init();
    if (curl) {
        // Establece la URL de destino
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        // Establece el método de solicitud como POST
        curl_easy_setopt(curl, CURLOPT_POST, 1L);

        // Establece los datos JSON en el cuerpo de la solicitud
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());

        // Realiza la solicitud POST
        curl_easy_perform(curl);

        // Limpia y libera los recursos de cURL
        curl_easy_cleanup(curl);
    }

    delete event_list;
    delete sys;

    exit(EXIT_SUCCESS);
}
