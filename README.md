# Szöllősi János (BC6X4X) szakdolgozat.

# Ütközésvizsgálat automatikus optimalizálása térbeli modellekhez.

# Leírás

> - A szakdolgozat célja egy olyan függvénykönyvtár létrehozása, amely automatikusan létrehoz egy beimportált 3 dimenziós modellhez egy "hitboxot", amely alapján kiszámíthatjuk az adott modell ütközését más modellekkel. A program C nyelvben íródott, az OpenGL és SDL2 függvénykönyvtárak segítségével. A program fő célja a modellek felbontása atomi szintre (háromszögekre), majd a háromszögek metszéspontjának kiszámítása.

> - A függvénykönyvtár tökéletes lehet kisebb játékok fejlesztéséhez, vagy egyéb fizikai szimulációk motorjaként. Mivel az ütközésvizsgálat atomi szinten történik a modell háromszögekre bontásával, így nagy az ütközésvizsgálat pontossága.


## Függvénykönyvtár használata:
> - collision_triangle.h importálása a programba.
> - check_collision funkció meghívása adott modellekre.
> - visszatérési értek: true, ha van ütközés, false, ha nincs ütközés.

## Függvénykönyvtár használata demo programmal:
> - start.bat elindítása a program nevezetű mappában.