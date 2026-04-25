# M5StickC Plus2 - ESP-IDF Activity Monitor

Projeto de estudo com ESP-IDF + M5Unified para leitura de IMU e exibição no display.


# 🚀 Fase 1 — Setup M5StickC Plus2 com ESP-IDF + M5Unified

## 🎯 Objetivo

Configurar um projeto mínimo funcional utilizando:

* ESP-IDF
* M5StickC Plus2
* Biblioteca M5Unified

Resultado esperado:

* Build funcionando
* Firmware rodando
* Tela exibindo dados do acelerômetro (IMU)

---

## 🧱 Pré-requisitos

* ESP-IDF instalado (v5.x)
* Python configurado pelo ESP-IDF
* Placa M5StickC Plus2 conectada via USB

---

## 📁 Estrutura do projeto

```
m5stickc-idf-activity-haptic/
├── CMakeLists.txt
├── sdkconfig
└── main/
    ├── CMakeLists.txt
    ├── idf_component.yml
    └── main.cpp
```

---

## ⚙️ Passo 1 — Criar projeto

```bash
idf.py create-project m5stickc-idf-activity-haptic
cd m5stickc-idf-activity-haptic
```

---

## 📦 Passo 2 — Adicionar dependência

```bash
idf.py add-dependency "m5stack/M5Unified"
```

---

## 🧾 Passo 3 — Configurar idf_component.yml

Arquivo: `main/idf_component.yml`

```yaml
dependencies:
  m5stack/M5Unified: "^0.2.13"
```

---

## 🧾 Passo 4 — Configurar CMakeLists.txt

Arquivo: `main/CMakeLists.txt`

```cmake
idf_component_register(
    SRCS "main.cpp"
    INCLUDE_DIRS "."
)
```

---

## 💻 Passo 5 — Criar main.cpp

Arquivo: `main/main.cpp`

```cpp
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <M5Unified.h>

extern "C" void app_main(void)
{
    auto cfg = M5.config();
    M5.begin(cfg);

    M5.Display.setRotation(1);

    while (1) {
        float ax, ay, az;

        M5.Imu.getAccel(&ax, &ay, &az);

        M5.Display.fillScreen(TFT_BLACK);
        M5.Display.setCursor(10, 10);
        M5.Display.setTextColor(TFT_WHITE);

        M5.Display.printf("AX: %.2f\nAY: %.2f\nAZ: %.2f\n", ax, ay, az);

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
```

---

## 🔨 Passo 6 — Build e Flash

⚠️ Execute na raiz do projeto (não dentro de `/main`)

```bash
idf.py build
idf.py flash monitor
```

---

## ✅ Resultado esperado

Na tela do dispositivo:

```
AX: 0.01
AY: -0.02
AZ: 1.00
```

* Valores mudam ao movimentar o dispositivo
* Sistema está funcionando corretamente

---

## ⚠️ Problemas comuns (e soluções)

### ❌ Erro: `app_main undefined`

✔ Solução:

```cpp
extern "C" void app_main(void)
```

---

### ❌ Erro: M5Unified requires C++

✔ Solução:

* Renomear `main.c` → `main.cpp`

---

### ❌ Rodar build dentro de /main

✔ Solução:

Executar sempre na raiz do projeto:

```bash
idf.py build
```

---

## 🧠 Conclusão

Nesta fase você validou:

* Toolchain ESP-IDF
* Integração com M5Unified
* Uso da IMU (acelerômetro)
* Uso do display

👉 Base sólida para próximas fases.

---

## 🚀 Próxima fase

Implementar lógica de atividade:

* Detectar movimento
* Detectar inatividade
* Exibir alertas
* Ativar vibração

---

📌 Fase concluída com sucesso ✔
