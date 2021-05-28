# Bee worker
Programme standalone prenant en charge les requêtes distribuées sur un réseau Hive Entropy.

## Fonctionnement
- Écoute des demandes d'assistances entrantes.
- Routage des demandes d'assistances réceptionnées.
- Traitement de la demande (opération matricielle).
- Envoi du résultat du traitement.

## Installation
```
cmake -Bbuild/ -S. --target install
```

## Utilisation
Lancer le worker :
```
cmake -Bbuild/ -S.
./build/Bee
```
Lancer les tests :
```
cmake -Bbuild/ -S. --target BeeTest
./build/test/BeeTest
```

## Exemples