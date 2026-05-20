# Drivers USB

## CH340

El dispositivo ESP32 utilizado en este kit se comunica con tu laptop a través del chip **CH340**.

> **Descarga el driver SOLO si tu sistema operativo no reconoce el dispositivo al conectarlo.** En muchos casos (Windows 10/11 recientes, macOS reciente, la mayoría de distribuciones Linux modernas) el driver ya viene incluido en el sistema.

### Descarga oficial

Guía completa con descargas por sistema operativo:
https://learn.sparkfun.com/tutorials/how-to-install-ch340-drivers/windows-710

---

## Verificación

Conecta el dispositivo por USB-C antes de instalar cualquier driver y revisa:

**Windows:** `Administrador de dispositivos` → debe aparecer `USB-SERIAL CH340 (COMx)` en la sección "Puertos (COM y LPT)".

**macOS:** en Terminal ejecuta `ls /dev/cu.*` → debe aparecer `/dev/cu.wchusbserial*`.

**Linux:** ejecuta `dmesg | tail -20` tras conectar → debe aparecer `ch341 converter now attached to ttyUSB0` (o similar).

**Si aparece el dispositivo:** no necesitas instalar nada, ya estás listo.

**Si NO aparece:** descarga e instala el driver desde el enlace de arriba.

---

## Problemas frecuentes

### El dispositivo no aparece en ningún lado

Causa más común: el cable USB-C es **solo de carga**, no de datos. Usa el cable incluido en el kit MisDesarrollos o uno conocido de datos.

### Aparece con signo de admiración amarillo (Windows)

Falta el driver CH340. Descarga desde el enlace e instala como administrador.

### En Linux aparece el puerto pero al subir código sale "Permission denied"

Falta agregar tu usuario al grupo `dialout`:

```bash
sudo usermod -aG dialout $USER
```

Después cierra sesión y vuelve a iniciar.
