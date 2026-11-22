# MdMathView

Visor de archivos **Markdown (`.md`, `.txt`) con soporte matemático LaTeX** diseñado para la calculadora **Casio fx-9750GIII** (y modelos monocromáticos compatibles SH3/SH4 como fx-9860GII/GIII), escrito en C usando **fxSDK** y **gint**.

## Características
- **Explorador de archivos integrado:** Escanea la raíz (`/`) de la memoria de la calculadora en busca de archivos `.md` y `.txt`, e incluye una demo integrada.
- **Soporte para archivos de hasta 24 KB** con normalización automática de UTF-8 (tildes y `ñ` en español).
- **Markdown:** Encabezados (`#`, `##`, `###`) con soporte de ecuaciones inline, listas (`- `, `* `), citas (`> `), separadores (`---`), tablas con cuadrícula y limpieza de `**negrita**`/`*cursiva*`.
- **Matemáticas LaTeX (`$...$` y `$$...$$`):**
  - Fracciones `\frac{a}{b}`, raíces `\sqrt{...}`, superíndices (`^`), subíndices (`_`), `\vec{...}`, `\tilde{...}`, `\text{...}`.
  - Símbolos vectoriales dibujados desde cero: `\sum` (con límites apilados), `\int`, `\pi`, `\pm`, `\infty`, `\chi`, `\omega`, `\varepsilon`, `\nabla`, `\hbar`, `\lambda`, `\mu`, `\uparrow`, `\downarrow`.

## Instalación y Uso
1. Descarga `MdMathView.g1a` desde la sección **Releases** (`v0.1`).
2. Conecta tu Casio fx-9750GIII al PC por USB y selecciona **USB Flash**.
3. Copia `MdMathView.g1a` y tus archivos `.md` directamente en la **raíz** de la calculadora.
4. Desconecta de forma segura y abre **MdMathView** desde el menú principal.

### Controles
- **↑ / ↓:** Desplazar verticalmente (o moverse en el selector de archivos).
- **← / →:** Desplazar horizontalmente para líneas o ecuaciones extensas.
- **F1 / F2:** Página arriba / página abajo.
- **F3 / F4:** Reducir / aumentar el ancho de ajuste de línea (`124 px` a `360 px`).
- **EXE:** Abrir el archivo seleccionado.
- **EXIT:** Volver al selector de archivos.
- **MENU:** Volver al menú principal de Casio.

## Cómo compilar desde el código fuente
Requiere **Linux** o **Windows con WSL (Ubuntu)** y el toolchain de Planète Casio (`fxsdk`, `sh-elf-gcc`, `fxlibc`, `gint`).

```bash
# 1. Instalar dependencias del sistema (Ubuntu/WSL)
sudo apt-get update && sudo apt-get install -y git cmake make libpng-dev libusb-1.0-0-dev libudisks2-dev python3-pil

# 2. Instalar toolchain y gint mediante GiteaPC
git clone --depth 1 https://git.planet-casio.com/Lephenixnoir/GiteaPC.git
python3 GiteaPC/giteapc.py install -y Lephenixnoir/gint

# 3. Asegurar ~/.local/bin en el PATH y compilar
export PATH="$HOME/.local/bin:$PATH"
cd MdMathView
fxsdk build-fx
```

El archivo compilado `MdMathView.g1a` se generará en la raíz del proyecto.
