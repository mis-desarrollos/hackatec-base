# FAQ — Preguntas Frecuentes

## Sobre el kit y el dispositivo

### ¿Qué dispositivo entrega MisDesarrollos?
Un **ESP32 con display LCD TFT 1.14" integrado** (controlador ST7789, 135x240 píxeles), USB-C, WiFi 2.4 GHz y Bluetooth BLE. Es una placa lista para usar — no requiere protoboard ni cables sueltos para el display.

### ¿Qué incluye el kit?
1. El dispositivo ESP32 con LCD integrado.
2. Cable USB-C de datos (para programar y alimentar).
3. Acceso a este repositorio base.

### ¿Podemos usar nuestro propio hardware adicional?
Sí. Pueden agregar sensores, actuadores, módulos de comunicación, etc., conectándolos a los pines GPIO disponibles (ver [`PINOUT.md`](./PINOUT.md)). El criterio es que el ESP32 con LCD debe ser parte central de la solución.

### ¿Cuánto dura la batería?
El dispositivo **no tiene batería integrada**. Debe estar conectado por USB-C durante todo el demo. Si su solución requiere movilidad, pueden agregar una powerbank pequeña al kit.

### ¿Hay WiFi en el venue?
Sí, los equipos usan el WiFi del lugar. Confirmen con la organización las credenciales el día del evento. **Recuerden que solo funciona WiFi 2.4 GHz** — si la red corporativa tiene portal cautivo o es WPA-Enterprise, usen hotspot de celular. Ver [`WIFI.md`](./WIFI.md).

---

## Sobre el repositorio base

### ¿Cuál es el alcance de este repo?
Es un **punto de partida técnico** que elimina el setup del hardware. Incluye instrucciones de drivers, ejemplos funcionales y plantilla de proyecto. **No** incluye lógica de negocio ni soluciones al reto.

### ¿En qué lenguaje deberíamos programar?
Soportamos **Arduino IDE (C++)** y **MicroPython**. El repo incluye ejemplos completos en ambos. Recomendación rápida:
- **Arduino:** mejor performance, comunidad más grande, librerías más maduras.
- **MicroPython:** iteración más rápida, sintaxis más simple, mejor para equipos con experiencia Python pero no C++.

Decidan en su equipo y manténganse en uno solo durante el hackathon.

### ¿Podemos modificar el código del repo base?
Sí. Es un punto de partida, no una restricción. Copien la plantilla de `templates/` y constrúyanla a su gusto.

### ¿Podemos publicar nuestra solución en GitHub?
Sí, totalmente.

---

## Sobre conexión y desarrollo

### Mi laptop no detecta el dispositivo
Causa más común: cable USB-C **solo de carga**. Usen el cable que viene con el kit. Si persiste, ver [`../DRIVERS.md`](../DRIVERS.md).

### ¿Funciona en macOS / Linux / Windows?
Sí en los tres. Las instrucciones de driver y configuración varían por SO, ver [`SETUP.md`](./SETUP.md).

### ¿Puedo usar mi propio framework (Rust, Zephyr, ESP-IDF directo, etc.)?
Sí, pero el soporte técnico solo cubre Arduino IDE y MicroPython. Si su equipo elige otra ruta, asuman el riesgo de debugging por su cuenta.

---

## Sobre comunicación

### ¿Cómo enviamos datos del dispositivo al backend?
HTTP es lo más simple y está cubierto en el ejemplo `http-client/`. Alternativas:
- **WebSockets** — bidireccional, baja latencia. Lib Arduino: `arduinoWebSockets`.
- **MQTT** — estándar IoT, broker centralizado. Lib Arduino: `PubSubClient`.
- **BLE** — directo al celular sin internet. Lib Arduino: `BLEDevice` (built-in en core ESP32).

### ¿Necesitamos un backend propio?
Si, ejmplos:
- **httpbin.org** para validar que el envío funciona (lo usamos en los ejemplos).
- **Firebase / Supabase** para almacenamiento sin servidor.
- **n8n** o **Zapier** para webhooks simples.
- Un Google Sheets como "base de datos" vía Apps Script.

### ¿El dispositivo puede recibir comandos del servidor?
Sí, tres patrones típicos:
1. **Polling HTTP** — el dispositivo pregunta al servidor periódicamente (lo más simple).
2. **WebSocket** — el servidor empuja mensajes al dispositivo cuando hay algo.
3. **MQTT** — pub/sub, escala mejor con muchos dispositivos.

### ¿Cuántas peticiones por segundo soporta?
Realistamente: ~5-10 requests HTTP por segundo. Si necesitan más throughput, agrupen datos en lotes (batch) o usen WebSocket.

---

## Sobre WiFi

### Quiero credenciales configurables sin recompilar
Opciones:
1. **`WiFiManager`** — librería Arduino que crea un portal cautivo para configurar SSID/password desde el celular. Útil para demo "plug and play".
2. **NVS (Non-Volatile Storage)** — guarda credenciales en flash, las lee al arrancar. Requiere algún mecanismo inicial para escribirlas.

Para hackathon, lo hardcoded (como en los ejemplos) suele ser suficiente.

### El ESP32 NO se conecta a la WiFi pero sí a mi hotspot
Causas típicas:
- Tu WiFi es 5 GHz (el ESP32 **solo soporta 2.4 GHz**).
- Tu router está en modo "guest network" con aislamiento.
- Tu router usa WPA3 puro (no compatible con todas las versiones del firmware).

Ver [`WIFI.md`](./WIFI.md) para detalles.

### ¿Funciona sin WiFi?
Sí, para casos offline. Pueden:
- Guardar datos localmente en flash (`Preferences` en Arduino, archivos en MicroPython).
- Comunicarse por BLE con el celular.
- Mostrar datos directamente en la pantalla.

---

## Sobre rendimiento y memoria

### ¿Cuánta RAM tengo disponible?
- Arduino: ~250 KB libres después de cargar el sketch.
- MicroPython: ~100 KB libres (el intérprete ocupa parte).

Para hackathon es más que suficiente. Si llegan al límite, posiblemente están guardando demasiados datos en RAM (usen flash con archivos).

### ¿Cuánto flash tengo disponible?
4 MB total. El bootloader y el firmware ocupan ~1 MB. Quedan ~3 MB para el sketch y archivos de usuario.

### Mi dispositivo se reinicia solo
Ver [`TROUBLESHOOTING.md`](./TROUBLESHOOTING.md) sección "El dispositivo se reinicia solo (bootloop)".

---

## Sobre los ejemplos del repositorio

### ¿Por qué los ejemplos Arduino usan Adafruit_ST7789 y no TFT_eSPI?
Adafruit_ST7789 es más simple de configurar (no requiere editar archivos internos de la librería). Para hackathon priorizamos facilidad de setup sobre máximo rendimiento.

Si tu solución requiere dibujado muy rápido (animaciones complejas, juegos, etc.), TFT_eSPI es ~3-5x más rápida pero requiere configuración manual de `User_Setup.h`. Queda a discreción del equipo migrar si lo necesitan.

### ¿Por qué MicroPython requiere un firmware especial?
El módulo `st7789` que usa el driver para esta pantalla es un módulo **C compilado**, no Python puro. Por eso debe venir incluido directamente en el firmware. El firmware estándar de MicroPython no lo incluye; por eso usamos el `GENERIC-7789` del repo de Russ Hughes que sí lo trae precompilado.

Existen drivers Python puro alternativos pero son significativamente más lentos.

### ¿Puedo combinar Arduino y MicroPython?
No al mismo tiempo. Cada uno requiere un firmware/bootloader diferente. Lo que **sí** puedes hacer es cambiar entre uno y otro reflasheando el dispositivo cuando quieras experimentar.

---

## Soporte durante el evento

### ¿Qué cubre el soporte técnico?
- Problemas de drivers y conexión USB.
- Errores en la compilación o ejecución de los ejemplos del repo.
- Hardware defectuoso o cable malo (cambio inmediato).

### ¿Qué NO cubre el soporte?
- Escribir código de la lógica de su solución.
- Debugging de código que su equipo ya modificó significativamente.
- Configuración de servicios externos (Firebase, AWS, etc.).