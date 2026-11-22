# Resumen de Clase: Susceptibilidad de Primer Orden, Absorción, Dispersión y Aplicaciones en Fibra Óptica

**Profesor:** Juan Diego Zapata  
**Temática principal:** Interacción luz-materia en medios dieléctricos y semiconductores, modelo clásico del oscilador de Lorentz, susceptibilidad eléctrica compleja ($\chi^{(1)}$), espectros de absorción, dispersión cromática y transmisión por fibra óptica.

---

## 1. Materiales Dieléctricos e Interacción Luz-Materia

### 1.1 Naturaleza de los dieléctricos
* **Estructura electrónica:** Los materiales dieléctricos (aislantes) tienen sus orbitales y suborbitales ($s, p, \dots$) completamente llenos. Al no poseer electrones libres, la conducción eléctrica a través del material es muy difícil.
* **Amorfo vs. Cristalino:** Un cristal posee un arreglo periódico de átomos o moléculas. En contraste, materiales como el vidrio o la fibra óptica de sílice ($\text{SiO}_2$) son **amorfos**. Convertir un material amorfo en cristalino altera drásticamente sus propiedades ópticas (como su transmitancia y espectro de absorción).

### 1.2 Polarización y funciones de transferencia
Cuando un material dieléctrico (eléctricamente neutro) se somete a un campo eléctrico externo $\vec{E}$ (estático DC o armónico AC), las cargas se separan. Sobre cada electrón actúa la fuerza eléctrica:

$$\vec{F}_e = -e\vec{E} = m\vec{a}$$

Los electrones se aceleran en dirección opuesta al campo eléctrico, generando **dipolos eléctricos** que polarizan el material. Para describir esta interacción se emplean funciones de transferencia equivalentes: la polarización ($P$), la susceptibilidad de primer orden ($\chi^{(1)}$), el índice de refracción ($n$) y la permitividad ($\varepsilon$).

En el enfoque semiclásico del curso:
* Los **campos electromagnéticos** obedecen las **ecuaciones de Maxwell**.
* La **materia** (dieléctricos, metales, semiconductores) obedece la **mecánica cuántica** (niveles de energía cuantizados).

### 1.3 Aproximación de Óptica Lineal
En su forma escalar, la polarización $P$ inducida en un material se expresa como una serie de potencias del campo eléctrico $E$:

$$P = \varepsilon_0 \chi^{(1)} E + \varepsilon_0 \chi^{(2)} E^2 + \varepsilon_0 \chi^{(3)} E^3 + \dots$$

Donde $\varepsilon_0$ es la permitividad del vacío y $\chi^{(n)}$ son las susceptibilidades de orden $n$. En el régimen de **óptica lineal** (el de mayor contribución cuando no hay fenómenos no lineales), solo se conserva el primer término:

$$P^{(1)} = \varepsilon_0 \chi^{(1)} E$$

Esto equivale a un **sistema lineal** (Entrada $E \rightarrow$ Función de transferencia $\chi^{(1)} \rightarrow$ Salida $P$), en el cual la longitud de onda ($\lambda$) de la luz no cambia al atravesar el material.

---

## 2. Cálculo de la Susceptibilidad de Primer Orden ($\chi^{(1)}$): Modelo de Lorentz

Para encontrar una expresión analítica de $\chi^{(1)}$, se modela la interacción entre el electrón y el núcleo positivo como un sistema **masa-resorte**, donde el núcleo ejerce una fuerza restauradora elástica $F_r = kr$ (con constante elástica $k$).

### 2.1 Caso Estacionario (Campo DC)
Si el campo eléctrico no varía con el tiempo ($\frac{\partial E}{\partial t} = 0$), el sistema está en equilibrio de fuerzas:

$$k r = -e E \implies r = -\frac{e E}{k}$$

La polarización macroscópica del material se define a partir del momento dipolar por unidad de volumen:

$$P = -N e r$$

Donde $N$ es el número de electrones (o dipolos) por unidad de volumen y $e$ es la carga del electrón. Sustituyendo $r$:

$$P = -N e \left(-\frac{e E}{k}\right) = \frac{N e^2}{k} E$$

Igualando con la definición lineal $P = \varepsilon_0 \chi_1 E$, se obtiene la **susceptibilidad estática** ($\chi_0$):

$$\chi_1(\omega = 0) = \chi_0 = \frac{N e^2}{\varepsilon_0 k}$$

---

### 2.2 Caso Dinámico (Campo Oscilante AC)
Consideremos ahora un campo eléctrico armónico dependiente del tiempo con frecuencia angular $\omega$:

$$E(t) = E_0 e^{-i\omega t}$$

Al oscilar dentro de la red atómica, el electrón interactúa y colisiona con otros electrones, iones y vibraciones de la red (fonones). Esto introduce una **fuerza de amortiguamiento** análoga a un fluido viscoso, proporcional a la velocidad: $m\gamma \frac{dr}{dt}$.

La ecuación de movimiento (oscilador armónico amortiguado y forzado) es:

$$m \frac{d^2 r}{dt^2} + m \gamma \frac{dr}{dt} + k r = -e E(t)$$

* **Análisis dimensional de $\gamma$:** Para que $m \gamma \frac{dr}{dt}$ tenga unidades de fuerza ($\text{masa} \times \text{aceleración}$), el factor de amortiguamiento $\gamma$ debe tener dimensiones de **frecuencia** ($\text{s}^{-1}$ o $\text{Hz}$). Físicamente, $\gamma$ es inversamente proporcional al tiempo medio entre colisiones (asociado al **camino libre medio** del electrón).

#### Solución en el dominio de la frecuencia
Se propone una solución armónica estacionaria de la forma $r(t) = r_0 e^{-i\omega t}$. Sus derivadas temporales son:

$$\frac{dr}{dt} = -i\omega r, \qquad \frac{d^2 r}{dt^2} = -\omega^2 r$$

Sustituyendo en la ecuación de movimiento:

$$(-m\omega^2 - i m \gamma \omega + k) r = -e E$$

Despejando $r$ y factorizando la masa $m$:

$$r = \frac{-e E}{m \left(\frac{k}{m} - \omega^2 - i \gamma \omega\right)}$$

Se define la **frecuencia natural o de resonancia** del sistema como:

$$\omega_0^2 = \frac{k}{m}$$

> **Interpretación cuántica de $\omega_0$:** Aunque el modelo es clásico, $\omega_0$ representa las frecuencias propias de transición entre los niveles cuantizados de energía de los átomos del material (por ejemplo, en el átomo de hidrógeno $E_n = -\frac{13.6\text{ eV}}{n^2}$).

Reescribiendo $r$:

$$r = \frac{-e E}{m (\omega_0^2 - \omega^2 - i \gamma \omega)}$$

Sustituyendo $r$ en $P = -N e r = \varepsilon_0 \chi^{(1)}(\omega) E$:

$$P = \frac{N e^2}{m (\omega_0^2 - \omega^2 - i \gamma \omega)} E$$

De aquí se despeja la **susceptibilidad de primer orden dependiente de la frecuencia**:

$$\chi^{(1)}(\omega) = \frac{N e^2}{\varepsilon_0 m} \cdot \frac{1}{\omega_0^2 - \omega^2 - i \gamma \omega}$$

Multiplicando y dividiendo por $\omega_0^2$, y recordando que $\chi_0 = \frac{N e^2}{\varepsilon_0 k} = \frac{N e^2}{\varepsilon_0 m \omega_0^2}$:

$$\chi^{(1)}(\omega) = \chi_0 \frac{\omega_0^2}{\omega_0^2 - \omega^2 - i \gamma \omega}$$

*(Verificación de consistencia: cuando $\omega = 0$, se recupera exactamente la susceptibilidad estática $\chi^{(1)}(0) = \chi_0$).*

---

## 3. Parte Real e Imaginaria de la Susceptibilidad

Al multiplicar numerador y denominador por el complejo conjugado $(\omega_0^2 - \omega^2 + i \gamma \omega)$, se separa $\chi^{(1)}(\omega)$ en su parte real y su parte imaginaria:

$$\chi^{(1)}(\omega) = \text{Re}[\chi^{(1)}(\omega)] + i\,\text{Im}[\chi^{(1)}(\omega)] = \chi'(\omega) + i \chi''(\omega)$$

1. **Parte Real (asociada al índice de refracción y la dispersión):**
   $$\chi'(\omega) = \frac{\chi_0 \omega_0^2 (\omega_0^2 - \omega^2)}{(\omega_0^2 - \omega^2)^2 + \gamma^2 \omega^2}$$

2. **Parte Imaginaria (asociada a la absorción del material):**
   $$\chi''(\omega) = \frac{\chi_0 \omega_0^2 \gamma \omega}{(\omega_0^2 - \omega^2)^2 + \gamma^2 \omega^2}$$

*(Ecuación inferida de conexión óptica: el índice de refracción complejo $\tilde{n}(\omega) = n(\omega) + i\kappa(\omega)$ se relaciona con la susceptibilidad mediante $\tilde{n}^2(\omega) = 1 + \chi^{(1)}(\omega)$, vinculando $\chi'$ con $n(\omega)$ y $\chi''$ con el coeficiente de extinción/absorción $\kappa(\omega)$).*

---

## 4. Análisis Físico de la Absorción ($\text{Im}[\chi^{(1)}(\omega)]$)

Analizando el comportamiento de $\chi''(\omega)$ respecto a la frecuencia de la luz incidente ($\omega$) y la frecuencia de resonancia del material ($\omega_0$):

* **Frecuencias muy altas ($\omega \gg \omega_0$):** El denominador crece como $\omega^4$, por lo que $\chi''(\omega) \rightarrow 0$.
* **Frecuencias muy bajas ($\omega \ll \omega_0$):** El numerador tiende a cero respecto a $\omega_0^4$, por lo que $\chi''(\omega) \rightarrow 0$.
* **En resonancia ($\omega = \omega_0$):** La absorción alcanza su **valor máximo**:
  $$\chi''(\omega_0) = \frac{\chi_0 \omega_0^3 \gamma}{\gamma^2 \omega_0^2} = \frac{\chi_0 \omega_0}{\gamma}$$

### Aproximación cerca de la resonancia (Curva Lorentziana)
Cuando $\omega \approx \omega_0$, se puede usar la factorización $\omega_0^2 - \omega^2 = (\omega_0 - \omega)(\omega_0 + \omega) \approx 2\omega_0(\omega_0 - \omega)$. Sustituyendo en $\chi''(\omega)$ se obtiene una función **Lorentziana**:

$$\chi''(\omega) \approx \frac{\chi_0 \omega_0}{4(\omega_0 - \omega)^2 + \gamma^2} \gamma$$

* El pico está centrado en $\omega = \omega_0$.
* El **ancho a media altura (FWHM)** de la curva es igual al factor de amortiguamiento $\gamma$.

### Implicaciones Físicas
* **Transparencia:** Si la frecuencia de la luz está lejos de $\omega_0$, el material absorbe muy poco y es transparente. Por ejemplo, el vidrio es transparente a la luz visible ($\sim 1\%$ de absorción), pero absorbe fuertemente en el **ultravioleta (UV)** donde están sus frecuencias de resonancia.
* **Absorción vs. Potencia:** Se deben diferenciar dos conceptos: *en qué región espectral* absorbe el material (determinado por $\omega_0$) y *cuánta potencia* absorbe (que escala con la intensidad incidente si la longitud de onda coincide con una banda de absorción).

---

## 5. Caracterización Experimental y Semiconductores (Perovskitas 2D)

### 5.1 Espectroscopía de Absorción y Transmitancia
Para medir experimentalmente el espectro de un material:
1. Se utiliza una **fuente de banda ancha** (luz blanca o láser de amplio espectro con muchas frecuencias) cuyo espectro de entrada $I_{\text{in}}(\lambda)$ se conoce.
2. La luz atraviesa la muestra del material (que puede poseer múltiples frecuencias de resonancia $\omega_{01}, \omega_{02}, \dots$).
3. Un **espectrómetro** mide la salida $I_{\text{out}}(\lambda)$. Las caídas pronunciadas en la transmitancia revelan los picos de absorción (resonancias).

### 5.2 Perovskitas 2D y Fotoluminiscencia
* Las **perovskitas 2D** son semiconductores de *bandgap* directo con espesores del orden de $1\text{ nm}$, donde los electrones están confinados a moverse en un plano (a diferencia de los materiales 3D).
* Tienen fuertes picos de absorción en el espectro visible (por ejemplo, en el verde alrededor de $532\text{ nm}$ y en el azul), lo que las hace ideales para **celdas solares, LEDs y láseres**.
* También exhiben **fotoluminiscencia**: al excitar el material con luz, los electrones suben a la banda de conducción y luego decaen a la banda de valencia emitiendo fotones con un perfil espectral tipo Lorentziano.

### 5.3 ¿Por qué la absorción en semiconductores decae con la longitud de onda ($\lambda$)?
La explicación física reside en la **densidad de estados** ($g(E)$), definida como el número de funciones de onda (estados disponibles) por unidad de energía:
1. La absorción ($\alpha$) es proporcional a la densidad de estados disponibles a los que puede saltar un electrón:
   $$\alpha \propto g(E)$$
2. Para fermiones en un cristal 3D, la densidad de estados escala con la raíz de la energía:
   $$g(E) \propto E^{1/2}$$
3. La energía de un fotón es inversamente proporcional a la longitud de onda:
   $$E = \frac{hc}{\lambda} \propto \frac{1}{\lambda}$$
4. En consecuencia, a mayor longitud de onda $\lambda$, menor es la energía $E$ y menor es la densidad de estados $g(E)$, por lo que la probabilidad de absorción disminuye:
   $$\alpha \propto \frac{1}{\lambda^m} \quad (\text{si } \lambda \uparrow \implies \text{Absorción } \downarrow)$$

---

## 6. Aplicaciones en Telecomunicaciones: Fibra Óptica

La fibra óptica es una **guía de onda dieléctrica** fabricada de sílice amorfa ($\text{SiO}_2$), con diámetros de núcleo típicos de $25\text{ }\mu\text{m}$ a $50\text{ }\mu\text{m}$ (multimodo) o menores (monomodo). Supera al cable coaxial al evitar pérdidas severas por efecto piel (*skin effect*) a altas frecuencias, inmunidad a interferencias electromagnéticas y un ancho de banda del orden de los $\text{THz}$.

### 6.1 Atenuación y Ventana C (Parte Imaginaria de $\chi^{(1)}$)
* **Ventana C ($1530\text{ nm} - 1550\text{ nm}$):** Es la región estándar de telecomunicaciones porque la sílice presenta aquí su **mínimo absoluto de atenuación**, aproximadamente $0.2\text{ dB/km}$ en $1550\text{ nm}$.
* **Pico de agua ($\sim 1300\text{ nm} - 1380\text{ nm}$):** Aparece un pico de absorción debido a impurezas de moléculas de agua ($\text{OH}^-$) atrapadas durante la fabricación, cuyas frecuencias de vibración molecular entran en resonancia en esa banda.
* **Escala logarítmica de potencia (dBm):** Recordatorio de la relación entre potencia en vatios y $\text{dBm}$:
  $$P_{\text{dBm}} = 10 \log_{10}\left(\frac{P}{1\text{ mW}}\right)$$
  De modo que $0\text{ dBm} = 1\text{ mW} = 10^{-3}\text{ W}$.
* Debido a la creciente demanda de ancho de banda (impulsada por tecnologías como la IA), las redes ópticas están comenzando a expandirse de la **banda C** hacia la **banda L**.

### 6.2 Dispersión Cromática (Parte Real de $\chi^{(1)}$)
* Debido a que el índice de refracción $n(\omega)$ depende de la frecuencia (a través de $\chi'(\omega)$), la fibra es un **medio dispersivo**.
* Un pulso óptico está compuesto por múltiples componentes espectrales (frecuencias). En un medio dispersivo, cada componente viaja a una **velocidad de grupo** ($v_g = \frac{d\omega}{dk}$) diferente (por ejemplo, componentes rojas y azules viajan a distintas velocidades).
* **Consecuencia:** El pulso se ensancha temporalmente al propagarse. En transmisiones digitales (por ejemplo, modulación NRZ de bits `1` y `0`), los pulsos adyacentes se superponen provocando **Interferencia Intersimbólica (ISI)**, lo que corrompe la información recibida.

### 6.3 Compromiso Atenuación vs. Dispersión y Soluciones
| Longitud de onda | Atenuación (Pérdida) | Dispersión Cromática (Fibra estándar) |
| :--- | :--- | :--- |
| $\sim 1310\text{ nm}$ | Moderada | Cercana a cero ($D \approx 0$) |
| $\sim 1550\text{ nm}$ (Ventana C) | Mínima ($\sim 0.2\text{ dB/km}$) | Alta |

Dado que la dispersión es un **fenómeno lineal** (su función de transferencia es invertible), existen dos estrategias principales en ingeniería para mitigarla:
1. **Compensación de dispersión:** Utilizar fibras compensadoras de dispersión (DCF) o procesamiento digital que aplican la transformada inversa para restaurar el ancho original del pulso.
2. **Fibras de dispersión desplazada:** Diseñar el perfil de la fibra para desplazar el punto de dispersión cero hacia la ventana de mínima atenuación ($1550\text{ nm}$).
