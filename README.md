# buzon-sim

Implementación de un simulador básico para el protocolo asociado a Buzón APS, que consiste, en palabras simples, en un protocolo de intervención a pacientes adultos mayores para abordar hospitalizaciones evitables en Chile. Para esto existen gestores de casos en los CESFAM, quienes evalúan el riesgo de los pacientes y en base a esto, si el riesgo final - el mayor entre el riesgo clínico y social - es alto o moderado, estos actores gestionan horas médicas, de exámenes, de servicio social y/o de atención psicológica dependiendo de las necesidades del paciente. Este simulador realiza una simulación basada en agentes y tiene por objetivo simular este proceso en términos básicos.


Para ejecutar pruebas, ejecutar el comando ``make`` en la carpeta raíz y posteriormente ejecutar el script run.sh mediante el comando ``bash run.sh``. Los archivos resultantes con los logs quedarán en la carpeta ``/out``.

La documentación asociada se encuentra el el archivo .pdf /docs/informe_buzon_sim_FlorenciaCorvalan.pdf y en la documentación generada mediante Doxygen se encuentra en la ruta ``/buzon-sim/docs/html/index.html``.
