  // CheckRain():
  
  int water = 0;
  
  // CheckTemperature() & CheckHumidity():
  
  #include <dht11.h> // charger la librairie
  
  dht11 DHT11; // Créer le capteur pour humidité/température
  
  #define DHT11PIN 6 // Définir la pin utilisée par ce capteur
  
  //CheckWind():
  
  unsigned long timeStart;    // Chrono start
  unsigned long timeEnd;      // Chrono stop
  
  float buttonPushCounter = 0;  // Compte le nombre d'impulsion de l'anemometre avec la led 
  float pi = 3.14159265359;     // Nombre pi
  float RayonDesBras = 0.07 ;  //en metres  
  int buttonState = 0;         // Etat actuel de l'impulsion 
  int lastButtonState = 0;     // Etat precedent de l'impulsion
  int FEtalonage = 1;          // Frequence d'etalonnage de l'anemometre

  //LCD():
  #include <LiquidCrystal.h> // on importe la bibliothèque
  LiquidCrystal monEcran(12,11,5,4,3,2); // on crée l'objet écran
  
  //*********** Creation des fonctions ***********\\
  

  //*********** Fonction detection de pluie ***********\\
  
   bool CheckRain()
  {
      pinMode(water, INPUT);   // Determine le pin du capteur et qu'il recoit des données
      int indicateur = analogRead(water);  // on lis une valeur analogique sur ce pin
      if (indicateur = 0);
      return indicateur == 0;   // == return true; return false;
  }
  
  //*********** Fonction detection de temperature ***********\\
  
  bool CheckTemperature()
  {
      int temperature = DHT11.read(DHT11PIN); // Lecture du capteur
  
      Serial.print("Temperature (°C): "); // Température
      Serial.println((float)DHT11.temperature, 2);  // on montre la temp dans l'invite de commande
       
      if (temperature < 40)
        return true;
      return false;
  }
  
  //*********** Fonction detection de l'humidité ***********\\
  
  bool CheckHumidity()
          {
              int humidity = DHT11.read(DHT11PIN); // Lecture du capteur
  
    Serial.print("Humidite (%): "); // Humidité
    Serial.println((float)DHT11.humidity, 2);  // on montre l'humidité dans l'invite de commande
             
              if (humidity > 0.6)
                  return true;
              return false;
          }
  
  //*********** Fonction detection de la vitesse du vent ***********\\
  
   bool CheckWind()
          {
               pinMode(7,INPUT);   // Determine le pin du capteur
               timeStart = millis(); // Debut du chrono
                
               buttonPushCounter = 0;   // Compte le nb d'implulsions
               while (timeEnd < timeStart + 1000)  {     // Permet de prendre le bn d'implusion pendant 1 seconde
                buttonState = digitalRead(7);             // On lit le pin 7 tant qu'on ne sort pas de la boucle while
                 if (buttonState != lastButtonState) {    
                   if (buttonState == HIGH) {
                     buttonPushCounter++;                 // On incremente le nb d'implusions
                    }
                    lastButtonState = buttonState;        // Derniere valeur devient la valeur initiale du programme
                   }
                   timeEnd = millis();  // fin du chrono
                  }
                  Serial.print("Nb de tour en une seconde = "),Serial.println(buttonPushCounter / 2);
                                      // Il faut 2 impulsion pour faire un tour d'anemometre donc on divise par 2\\
                    
  
  Serial.print("Vitesse du vent en m/s = "),Serial.println(2*pi*RayonDesBras*(buttonPushCounter / 2.0)*FEtalonage);
    
  Serial.print("Vitesse du vent en km/h = "),Serial.println((2*pi*RayonDesBras*(buttonPushCounter / 2.0)*FEtalonage)*3.6);
     
    timeStart = millis();
  
   float wind = ((2*pi*RayonDesBras*(buttonPushCounter / 2.0)*FEtalonage)*3.6);
   
              if (wind > 90)
                  return true;
              return false;
          }


  //*********** Fonction affichage ecran LCD ***********\\
  
  void LCD (int x)
  {
      
      monEcran.begin(16,2); //on initialise la communication avec 16 colonnes et deux lignes
      monEcran.clear(); // on efface l'écran
      monEcran.print("Il faut Rouler a :"); // On ecrit
      monEcran.setCursor(0,1); //on passe à la ligne suivante
      monEcran.print(x ,"km/h"); // on donne la vitesse

    
  }


  //*********** Creation des fonctions principales deux cas , oui et non ***********\\
  
  int Yes()
          {
              bool temperature = CheckTemperature();
              if (temperature)
                  return 110;
              return 120;
          }
  
  
  int No()
          {
              bool humidity = CheckHumidity();
   
                if (humidity) 
              {
                  bool temperature = CheckTemperature();
                  if (temperature)
                      return 110;
                  bool wind = CheckWind();
                  if (wind)
                      return 110;
                  return 130;
              }
              else
              {
                  bool wind = CheckWind();
                  if (wind)
                      return 110;
                  return 130;
              }
          }
  


  //*********** Utilisation des fonctions pour arriver aux resultats recherchés ***********\\
  
  void setup() {
    
    Serial.begin(9600);
  
  }
  
  void loop() {
  
    bool Rain = CheckRain();
   
    if (Rain){
      int x = Yes();
      LCD(x);
    }
    else{
      int x = No();
      LCD(x);
    }
    delay (10000);
  }
