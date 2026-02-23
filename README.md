# S.C.S

Sistema Cabin Security integra Arduino, sensores ambientais, biometria, teclado e interface web para controlo de acesso inteligente. Permite autenticação por senha ou impressão digital, monitoramento de temperatura, humidade e chama, registo em base de dados MySQL e painel web administrativo em tempo real.

---

# 📘 README – Cabin Security System

## 📖 Visão Geral

O **Cabin Security** é um sistema completo de segurança embarcado + web que combina:

* 🔐 Controlo de acesso (senha + impressão digital)
* 🌡 Monitoramento ambiental (temperatura, humidade, chama)
* 📊 Armazenamento em base de dados
* 🌐 Interface web administrativa
* 🔄 Comunicação Arduino → Python → API PHP → MySQL

Arquitetura híbrida IoT + Web.

---

# 🏗️ Estrutura do Projeto

```
cabin-security/
├── index.html
├── admin.html
├── operator.html
├── manage_operators.html
├── monitor_operators.html
├── device_status.html
├── serial_to_api.py
├── cabin_security.ino
├── database.sql
└── api/
```

---

# ⚙️ Arquitetura do Sistema

### 1️⃣ Arduino (cabin_security.ino)

Responsável por:

* Leitura do:

  * DHT11 (temperatura e humidade)
  * Sensor de chama
  * Sensor biométrico
  * Teclado matricial
* Controle do:

  * Motor da porta (L293D)
  * Buzzer
  * LCD I2C
* Envio de dados via Serial

Eventos enviados:

```
TEMP:xx,HUMID:xx,FLAME:NORMAL
EVENT:DOOR:UNLOCK
EVENT:AUTH_FAIL
EVENT:ENROLL,ID:x
```

---

### 2️⃣ Python Bridge (serial_to_api.py)

Função:

* Lê dados da porta serial
* Processa eventos
* Envia dados para API PHP via HTTP POST

Fluxo:

```
Arduino → Serial → Python → API → MySQL
```

---

### 3️⃣ Backend PHP (api/)

Endpoints disponíveis:

| Endpoint            | Função                  |
| ------------------- | ----------------------- |
| login.php           | Autenticação            |
| logout.php          | Encerrar sessão         |
| add_operator.php    | Adicionar operador      |
| remove_operator.php | Remover operador        |
| get_operators.php   | Listar operadores       |
| save_data.php       | Salvar sensores         |
| log_access.php      | Registar acessos        |
| get_sensors.php     | Buscar dados ambientais |
| get_door_status.php | Estado da porta         |

---

### 4️⃣ Interface Web

* `index.html` → Login
* `admin.html` → Painel administrador
* `operator.html` → Painel operador
* `device_status.html` → Estado do sistema

Interface moderna com CSS responsivo.

---

# 🛠️ Tecnologias Utilizadas

## 🔌 Hardware

* Arduino Mega
* Sensor DHT11
* Sensor de chama
* Sensor biométrico Adafruit
* Keypad 4x4
* LCD I2C
* Driver L293D
* Buzzer

## 💻 Software

* Arduino IDE
* Python 3
* PHP
* MySQL
* HTML5 + CSS3 + JavaScript
* XAMPP (Apache + MySQL)

---

# 🗄️ Banco de Dados

1. Abrir phpMyAdmin
2. Criar base de dados
3. Executar `database.sql`

---

# 🚀 Como Executar

## 1️⃣ Arduino

* Abrir `cabin_security.ino`
* Carregar no Arduino

## 2️⃣ Servidor Local

* Iniciar XAMPP
* Colocar pasta `cabin-security` em `htdocs`
* Importar `database.sql`

## 3️⃣ Python

Instalar dependências:

```bash
pip install pyserial requests
```

Executar:

```bash
python serial_to_api.py
```

---

# 🔐 Funcionalidades de Segurança

* Bloqueio após 4 tentativas erradas
* Autenticação biométrica
* Registro de acessos
* Monitoramento de incêndio
* Alertas sonoros
* Log completo no banco de dados

---

# 📡 Fluxo de Comunicação

```
Usuário → Keypad / Fingerprint
        ↓
Arduino
        ↓
Serial
        ↓
Python Script
        ↓
API PHP
        ↓
Banco de Dados
        ↓
Painel Web
```

---

# 📊 Possíveis Melhorias

* 🔔 Envio de alertas por Email/SMS
* 📱 Dashboard em tempo real com WebSockets
* 🔒 Hash de senha com bcrypt
* ☁️ Integração com servidor remoto
* 📈 Gráficos de monitoramento

---


