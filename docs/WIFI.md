# WiFi — Conectividad del Dispositivo

## ⚠️ Solo WiFi 2.4 GHz

El ESP32 soporta exclusivamente **WiFi 2.4 GHz**. Esta es la limitación más importante a tener en cuenta y la causa #1 de fallos de conexión en hackathons.

**El dispositivo NO puede conectarse a:**
- Redes WiFi 5 GHz
- Redes WiFi 6 GHz (Wi-Fi 6E)
- Redes con cifrado WPA3 puro (algunas implementaciones)

**Sí puede conectarse a:**
- Redes 2.4 GHz con WPA/WPA2/WPA2-Personal
- Redes abiertas (sin contraseña)
- Hotspots de celular en modo 2.4 GHz

---

## Verificar que tu red es 2.4 GHz

### Router típico de casa

Muchos routers modernos tienen dos bandas con nombres similares:
- `MiRed_2.4G` y `MiRed_5G` (separadas) — conecta a la 2.4G.
- `MiRed` única que combina ambas bandas (band steering) — el dispositivo verá la red pero podría no conectar si el router fuerza 5 GHz.

**Si tu router combina bandas:**
- Entra a la configuración del router (típicamente `192.168.1.1` o `192.168.0.1`).
- Busca "Smart Connect", "Band Steering" o "Banda dual" y desactívalo temporalmente.
- O crea una SSID separada solo para 2.4 GHz.

### Hotspot del celular

**iPhone:**
1. `Ajustes → Compartir Internet`.
2. Activa `Maximizar Compatibilidad` (esto fuerza 2.4 GHz).
3. El hotspot aparecerá visible al ESP32.

**Android (Samsung, Google Pixel, etc.):**
1. `Ajustes → Conexiones → Punto de acceso WiFi`.
2. En `Configurar punto de acceso → Banda`, selecciona **`2.4 GHz`**.
3. Guarda y activa el hotspot.

> Verifica desde otra computadora o celular que efectivamente la red aparece como 2.4 GHz antes de intentar con el ESP32.

---

## Limitaciones adicionales

### NO soporta portales cautivos

Redes corporativas o universitarias que requieren login web (típicamente WiFi de hoteles, cafés, campus) **no funcionan**. El ESP32 puede asociarse a la red pero no puede completar el login HTML.

**Solución:** usa el hotspot de tu celular durante el hackathon.

### NO soporta WPA2-Enterprise (.1x) sin configuración avanzada

Si la red usa autenticación con usuario+contraseña (no contraseña única), técnicamente se puede configurar pero requiere código adicional con certificados. No está cubierto por los ejemplos del repo.

### Caracteres especiales en SSID/contraseña

Algunos caracteres pueden causar problemas:
- Espacios al inicio o final de SSID
- Caracteres no-ASCII (acentos, ñ, emojis)
- Comillas dobles o caracteres de escape

**Recomendación:** usa SSIDs con solo letras, números, guiones y guiones bajos.

### Intensidad de señal

El ESP32 funciona bien hasta aproximadamente **-75 dBm**. Si tu RSSI es peor que -80 dBm, la conexión va a ser inestable:

| RSSI | Calidad | Comentario |
|---|---|---|
| -30 a -50 dBm | Excelente | Junto al router |
| -50 a -70 dBm | Buena | Trabajo normal |
| -70 a -80 dBm | Aceptable | Puede haber reconexiones |
| Peor que -80 dBm | Mala | Inestable, considera acercarte |

---

## Cómo conectar el dispositivo

Los ejemplos `wifi-setup/` (Arduino y MicroPython) tienen el patrón ya implementado. Solo necesitas editar las credenciales en el código.

### Arduino (`wifi-setup/arduino/wifi_setup.ino`)

```cpp
const char* WIFI_SSID     = "TU_RED_WIFI";
const char* WIFI_PASSWORD = "TU_PASSWORD";
```

### MicroPython (`wifi-setup/micropython/main.py`)

```python
WIFI_SSID     = "TU_RED_WIFI"
WIFI_PASSWORD = "TU_PASSWORD"
```

---

## Estrategia de credenciales para el evento

**Recomendación durante el HackaTec:**

1. **Para desarrollo en tu lugar de trabajo:** usa tu WiFi local 2.4 GHz.
2. **Para demo el día del evento:** prepara DOS opciones:
   - **Opción A:** WiFi del venue (confirma con organización que es 2.4 GHz sin portal cautivo).
   - **Opción B (respaldo):** hotspot de un celular del equipo configurado en 2.4 GHz.
3. **Cambiar credenciales:** edita el código, recompila y vuelve a subir.

> Si tu solución requiere mostrar varios cambios de red en vivo, considera implementar `WiFiManager` (librería Arduino) que crea un portal de configuración por celular. No está cubierto por los ejemplos base pero es una mejora opcional.

---

## Troubleshooting de WiFi

📄 Ver [`TROUBLESHOOTING.md`](./TROUBLESHOOTING.md) sección "WiFi no conecta".

---

## Bluetooth como alternativa

El ESP32 también soporta **Bluetooth Classic + BLE 4.2**. Si tu solución no requiere internet, puedes comunicarte directamente con un celular vía BLE sin necesidad de WiFi.

Ventajas:
- No depende de redes externas
- Latencia muy baja (<50 ms)
- Funciona incluso si el venue no tiene WiFi disponible

Librerías:
- Arduino: `BLEDevice` (incluida en el core ESP32, no requiere instalación adicional)
- MicroPython: `bluetooth` (módulo built-in)

Bluetooth no está cubierto por los ejemplos base de este repo pero es una opción válida para equipos que quieran independencia de WiFi.
