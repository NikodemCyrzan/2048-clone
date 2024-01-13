Indeks - s30161

Niestety po wielu próbach nie udało się stworzyć dzałającego pliku exe przy pomocy Dev-C++ z wbudowanym ncursesw. 

Kompilowanie pliku main.cpp przy użyciu funkcji Compile & Run, tworzyło plik exe, program uruchamiał się poprawnie, ale po uruchomieniu przez plik pokazywał się błąd sygnalizujący o braku biblioteki ncurses.
Kompilowanie całego projektu zatrzymywało się na błędzie pliku Makefile.win.


Lista funkcji do zaakceptowania:
- wykonywanie ruchów na planszy za pomocą klawiszy WSAD,
- po naciśnięciu danego klawisza, wszystkie liczby przemieszczają się w danym kierunku i dodają się zgodnie z oryginałem,
- z każdym ruchem na planszy pojawia się dodatkowa liczba,
- jeżeli niemożliwe jest wykonanie ruchu w danym kierunku i jeśli gracz zechce wykonać ruch w tym kierunku, nie zostanie on wykonany i nie pojawi się dodatkowa liczba,
- łączenie takich samych liczb dodaje punkty do licznika "Score",
- jeżeli niemożliwe jest wykonanie jakiegokolwiek ruchu, pojawia się napis "GAME OVER",
- każda liczba ma swój indywidualny kolor, w celu poprawienia widoczności.
