# Bee worker
Programme standalone prenant en charge les requêtes distribuées sur un réseau Hive Entropy.

## Fonctionnement
- Écoute des demandes d'assistances entrantes.
- Routage des demandes d'assistances réceptionnées.
- Traitement de la demande (opération matricielle).
- Envoi du résultat du traitement.

## Utilisation

### Initialiser le projet CMake
```bash
cmake -Bbuild/ -S.
```

### Construire et lancer le worker
```bash
cmake --build build/ --target Bee
./build/Bee [Addresse IP LAN du worker]
```

### Lancer les tests
```
cmake -Bbuild/ -S. --target BeeTest
./build/test/BeeTest
```

## Exemples