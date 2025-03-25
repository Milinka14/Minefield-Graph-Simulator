#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct bomba {
  float x;
  float y;
  float rad;
} Bomba;

typedef struct node {
  int indeks;
  struct node *next;
} Node;

typedef struct ListaSus {
  Bomba trenutna;
  Node *head;
} ListaSus;

void dodajGranu(ListaSus *graph, int cvor, int susjed) {
  Node *temp, *temp2;
  Node *pocetak = graph[cvor].head;
  if (graph[cvor].head == NULL) {
    temp = (Node *)malloc(sizeof(Node));
    graph[cvor].head = temp;
    temp->indeks = susjed;
    temp->next = NULL;
  } else {
    while (pocetak->next != NULL) {
      if (pocetak->indeks == susjed) {
        printf("VEC POSTOJI VEZA IZEMDJU OVIH CVOROVA!!!\n");
        return;
      }
      pocetak = pocetak->next;
    }
    if (pocetak->indeks == susjed) {
      printf("VEC POSTOJI VEZA IZEMDJU OVIH CVOROVA!!!\n");
      return;
    }
    temp2 = (Node *)malloc(sizeof(Node));
    pocetak->next = temp2;
    temp2->indeks = susjed;
    temp2->next = NULL;
  }
}

void brisanjeCvora(ListaSus **graph, int cvorBrisanje, int *dimenzije) {
  Node *prethodnik = NULL;
  int br = 0;
  for (int i = cvorBrisanje; i < *dimenzije - 1; i++) {
    (*graph)[i] = (*graph)[i + 1];
  }
  (*graph)[*dimenzije - 1].head = NULL;
  *graph = (ListaSus *)realloc(*graph, (*dimenzije - 1) * sizeof(ListaSus));
  *dimenzije = *dimenzije - 1;
  for (int i = 0; i < *dimenzije; i++) {
    Node *pocetak = (*graph)[i].head;
    prethodnik = NULL;
    while (pocetak != NULL) {
      if (pocetak->indeks == cvorBrisanje) {
        if (prethodnik != NULL) {
          prethodnik->next = pocetak->next;
        } else {
          (*graph)[i].head = pocetak->next;
        }
        free(pocetak);
        break;
      } else {
        prethodnik = pocetak;
        pocetak = pocetak->next;
      }
    }
    pocetak = (*graph)[i].head;
    while (pocetak != NULL) {
      if (pocetak->indeks > cvorBrisanje) {
        pocetak->indeks = pocetak->indeks - 1;
      }
      pocetak = pocetak->next;
    }
  }
}

void skiniteGranu(ListaSus *graph, int cvor, int susjed) {
  Node *temp;
  Node *prethodnik = NULL;
  Node *pocetak = graph[cvor].head;
  int br = 0;
  while (pocetak != NULL) {
    if (pocetak->indeks == susjed) {
      if (prethodnik != NULL) {
        prethodnik->next = pocetak->next;
      } else {
        graph[cvor].head = pocetak->next;
      }
      free(pocetak);
      ++br;
      break;
    }
    prethodnik = pocetak;
    pocetak = pocetak->next;
  }
  if (br == 0) {
    printf("Ne postoji veza izmedju cvorova %d i %d.\n", cvor, susjed);
  }
}

void printaj(int duzina, ListaSus *graph) {
  int i = 0;
  while (i < duzina) {
    Node *prvi = graph[i].head;
    printf("Svi susjedi cvora %d su : ", i);
    while (prvi != NULL) {
      printf("%d", prvi->indeks);
      prvi = prvi->next;
      if (prvi != NULL) {
        printf("-->");
      }
    }
    putchar('\n');
    i++;
  }
}

void obrisiGraf(ListaSus *graph, int duzina) {
  Node *pocetak, *temp;
  int i = 0;
  while (i < duzina) {
    pocetak = graph[i].head;
    while (pocetak != NULL) {
      temp = pocetak;
      pocetak = pocetak->next;
      free(temp);
    }
    graph[i].head = NULL;
    i++;
  }
  free(graph);
}

int main() {
  int odabir;
  printf("Koji zadatak zelite da odaberete: ");
  scanf("%d", &odabir);
  while (getchar() != '\n')
    ;
  if (odabir > 2) {
    return 0;
  }
  if (odabir == 2) {
    char line[20];
    float x2, y2, radius;
    ListaSus *graph = NULL;
    Node *temp1, *temp2;
    int i = 0, j = 0;
    double udaljenost, korjen1, korjen2;
    FILE *fajl = fopen("test3.txt", "r");
    if (fajl == NULL) {
      printf("Niste uspjesno otvorili fajl");
      return 1;
    }
    int broj;
    fscanf(fajl, "%d", &broj);
    graph = (ListaSus *) malloc(broj*sizeof(ListaSus));
    fgetc(fajl);
    while (i < broj) {
      fgets(line, sizeof(line), fajl); 
      sscanf(line, "%f %f %f", &x2, &y2, &radius);
      graph[i].trenutna.x = x2;
      graph[i].trenutna.y = y2;
      graph[i].trenutna.rad = radius;
      graph[i].head = NULL;
      i++;
    }

    fclose(fajl);
    for (i = 0; i < broj; i++) {
      for (j = 0; j < broj; j++) {
        Node *pocetak = graph[i].head;
        if (i != j) {
          korjen1 = graph[i].trenutna.x - graph[j].trenutna.x;
          korjen2 = graph[i].trenutna.y - graph[j].trenutna.y;
          udaljenost = sqrt(pow(korjen1, 2) + pow(korjen2, 2));
          if (udaljenost <= graph[i].trenutna.rad) {
            if (graph[i].head == NULL) {
              temp1 = (Node *)malloc(sizeof(Node));
              graph[i].head = temp1;
              temp1->indeks = j;
              temp1->next = NULL;
            } else {
              while (pocetak->next != NULL) {
                pocetak = pocetak->next;
              }
              temp2 = (Node *)malloc(sizeof(Node));
              pocetak->next = temp2;
              temp2->indeks = j;
              temp2->next = NULL;
            }
          }
        }
      }
    }
    for (int i = 0; i < broj; i++) {
      printf("Bomba %d: x=%.2f, y=%.2f, rad=%.2f\n", i, graph[i].trenutna.x,
             graph[i].trenutna.y, graph[i].trenutna.rad);
      Node *current = graph[i].head;
      printf("Neighbours: ");
      while (current != NULL) {
        printf("%d ", current->indeks);
        current = current->next;
      }
      printf("\n");
    } // DO OVDE KREIRANJE GRAFAAAAAAAAAAAAAAAAAAAA
    int od, kojubombu, qbrisanje, k, brojac, max, idk, brojukrugu;
    int effsusjedi[15];
    float raketax, raketay, raketarad, minx, miny, maxx, maxy, minx1, miny1,maxx1, maxy1, randomx, randomy, randomrad, povrsina;
    int *posjeceni;
    Node *pocetak1;
    int queque[30], front, rear, zacase3;
    posjeceni = (int *)calloc(broj, sizeof(int));
    while (1) {
      printf("\nMeni\n");
      printf("1.Detonirajte bombu.\n");
      printf("2.Nadjite bombu maks efikasnosti.\n");
      printf("3.Raketa.\n");
      printf("4.Ukupna povrsina.\n");
      printf("5.Kraj\n");
      printf("==========================\n");
      printf("Izaberite opciju: ");
      scanf("%d", &od);
      while (getchar() != '\n')
        ;
      switch (od) {
      case 1:
        for (i = 0; i < broj; i++) {
          posjeceni[i] = 0;
        }
        rear = -1;
        front = -1;
        printf("Koju bombu zelite da detonirate: ");
        scanf("%d", &kojubombu);
        while (getchar() != '\n')
          ;
        if (kojubombu > broj) {
          printf("Nepostojeca Bomba.");
          break;
        }
        posjeceni[kojubombu] = 1;
        printf("%.2f %.2f", graph[kojubombu].trenutna.x,
               graph[kojubombu].trenutna.y);
        queque[0] = kojubombu;
        rear += 1;
        front = 0;
        while (front != -1 && front <= rear) {
          qbrisanje = queque[front];
          front++;
          pocetak1 = graph[qbrisanje].head;
          while (pocetak1 != NULL) {
            if (posjeceni[pocetak1->indeks] == 0) {
              posjeceni[pocetak1->indeks] = 1;
              printf("||%.2f %.2f||", graph[pocetak1->indeks].trenutna.x,
                     graph[pocetak1->indeks].trenutna.y);
              ++rear;
              queque[rear] = pocetak1->indeks;
            }
            pocetak1 = pocetak1->next;
          }
        }
        break;
      case 2:
        idk = 0;
        max = -1;
        for (k = 0; k < broj; k++) {
          brojac = 0;
          rear = -1;
          front = -1;
          for (i = 0; i < broj; i++) {
            posjeceni[i] = 0;
          }
          posjeceni[k] = 1;
          queque[0] = k;
          rear += 1;
          front = 0;
          while (front != -1 && front <= rear) {
            qbrisanje = queque[front];
            front++;
            pocetak1 = graph[qbrisanje].head;
            while (pocetak1 != NULL) {
              if (posjeceni[pocetak1->indeks] == 0) {
                posjeceni[pocetak1->indeks] = 1;
                brojac++;
                rear++;
                queque[rear] = pocetak1->indeks;
              }
              pocetak1 = pocetak1->next;
            }
          }
          if (brojac > max) {
            max = brojac;
            idk = k;
          }
        }
        if (idk != -1) {
          // printf("%d    ", brojac);
          printf("Bomba maks efikasnosti: %d\n", idk);
          printf("x=%.2f, y=%.2f, rad=%.2f\n", graph[idk].trenutna.x,
                 graph[idk].trenutna.y, graph[idk].trenutna.rad);
        } else {
          printf("Sve bombe eksplodiraju samo sebe.\n");
        }
        break;

      case 3:
        printf("Unesite kordinate i radius rakete: ");
        scanf("%f %f %f", &raketax, &raketay, &raketarad);
        while (getchar() != '\n')
          ;
        for (i = 0; i < broj; i++) {
          posjeceni[i] = 0;
        }
        zacase3 = 0;
        for (k = 0; k < broj; k++) {
          korjen1 = graph[k].trenutna.x - raketax;
          korjen2 = graph[k].trenutna.y - raketay;
          udaljenost = sqrt(pow(korjen1, 2) + pow(korjen2, 2));
          if (udaljenost < raketarad) {
            zacase3++;
            if (posjeceni[k] == 0) {
              rear = -1;
              front = -1;
              posjeceni[k] = 1;
              printf("%.2f %.2f||", graph[k].trenutna.x, graph[k].trenutna.y);
              queque[0] = k;
              rear += 1;
              front = 0;
              while (front != -1 && front <= rear) {
                qbrisanje = queque[front];
                front++;
                pocetak1 = graph[qbrisanje].head;
                while (pocetak1 != NULL) {
                  if (posjeceni[pocetak1->indeks] == 0) {
                    posjeceni[pocetak1->indeks] = 1;
                    printf("||%.2f %.2f||", graph[pocetak1->indeks].trenutna.x,
                           graph[pocetak1->indeks].trenutna.y);
                    ++rear;
                    queque[rear] = pocetak1->indeks;
                  }
                  pocetak1 = pocetak1->next;
                }
              }
            }
          }
        }
        if (zacase3 == 0) {
          printf("Raketa ne ekplodira ni jednu bombu.");
        }
        break;
      case 4:
        povrsina = 0.0;
        for (i = 0; i < broj; i++) {
          posjeceni[i] = 0;
        }
        brojac = 0;
        rear = -1;
        front = -1;
        posjeceni[idk] = 1;
        queque[0] = idk;
        rear += 1;
        front = 0;
        effsusjedi[brojac] = idk;
        brojac++;
        while (front != -1 && front <= rear) {
          qbrisanje = queque[front];
          front++;
          pocetak1 = graph[qbrisanje].head;
          while (pocetak1 != NULL) {
            if (posjeceni[pocetak1->indeks] == 0) {
              posjeceni[pocetak1->indeks] = 1;
              effsusjedi[brojac] = pocetak1->indeks;
              brojac++;
              ++rear;
              queque[rear] = pocetak1->indeks;
            }
            pocetak1 = pocetak1->next;
          }
        }
        minx = 1000;
        maxx = -1000;
        miny = 1000;
        maxy = -1000;
        /*
        for (k = 0; k < brojac; k++) {
          i = effsusjedi[k];
          maxx1 = graph[i].trenutna.x + graph[i].trenutna.rad;
          if (maxx1 > maxx) {
            maxx = maxx1;
          }
          maxy1 = graph[i].trenutna.y + graph[i].trenutna.rad;
          if (maxy1 > maxy) {
            maxy = maxy1;
          }
          minx1 = graph[i].trenutna.x - graph[i].trenutna.rad;
          if (minx1 < minx) {
            minx = minx1;
          }
          miny1 = graph[i].trenutna.y - graph[i].trenutna.rad;
          if (miny1 < miny) {
            miny = miny1;
          }
        }*/
        // povrsina = (maxx - minx) * (maxy - miny);
        povrsina = 20 * 20;
        brojukrugu = 0;
        for (i = 0; i < 1000000; i++) {
          randomx = ((float)rand() / RAND_MAX) * 20 - 5;
          randomy = ((float)rand() / RAND_MAX) * 20 - 5;
          for (j = 0; j < brojac; j++) {
            korjen1 = graph[effsusjedi[j]].trenutna.x - randomx;
            korjen2 = graph[effsusjedi[j]].trenutna.y - randomy;
            udaljenost = sqrt(pow(korjen1, 2) + pow(korjen2, 2));
            if (udaljenost <= graph[effsusjedi[j]].trenutna.rad) {
              brojukrugu++;
              break;
            }
          }
        }
        povrsina = povrsina * ((float)brojukrugu / 1000000);
        printf("Povrsina je: %.2f", povrsina);
        break;
      case 5:
        return 0;
      }
    }
  } else {
    int od = 0;
    int cvorBrisanje;
    int i;
    ListaSus *graph = NULL;
    int dimenzije = -2, cvor, susjed;
    while (1) {
      printf("Meni\n");
      printf("1.Napravite graf\n");
      printf("2.Dodajte grane\n");
      printf("3.Uklonite granu \n");
      printf("4.Dodajte cvor\n");
      printf("5.Uklonite cvor\n");
      printf("6.Ispis grafa\n");
      printf("7.Brisanje grafa\n");
      printf("8.Kraj\n");
      printf("==========================\n");
      printf("Izaberite opciju: ");
      scanf("%d", &od);
      while (getchar() != '\n');
      switch (od) {
      case 1:
        printf("Unesite dimenzije grafa: ");
        scanf("%d", &dimenzije);
        while (getchar() != '\n')
          ;
        graph = (ListaSus *)calloc(dimenzije, sizeof(ListaSus));
        for (i = 0; i < dimenzije; i++) {
          graph[i].head = NULL;
        }
        printf("USPJESNO STE NAPRAVILI GRAF\n");
        break;
      case 2:
        if (dimenzije < 0) {
          printf("Niste napravili graf.\n");
          break;
        }
        printf("Unesite kojem cvoru dodajte, a zatim cvor koji dodajete: ");
        scanf("%d,%d", &cvor, &susjed);
        while (getchar() != '\n')
          ;
        if (cvor == susjed) {
          printf("Cvor ne moze da bude u vezi sam sa sobom.\n");
          break;
        }
        if (cvor >= dimenzije || susjed >= dimenzije) {
          printf("Unijeli ste nepostojece cvorove.\n");
          break;
        }
        dodajGranu(graph, cvor, susjed);
        break;
      case 3:
        if (dimenzije < 0) {
          printf("Niste napravili graf.\n");
          break;
        }
        printf("Unesite cvorove izmedju kojih zelite da obriste vezu: ");
        scanf("%d,%d", &cvor, &susjed);
        if (cvor >= dimenzije || susjed >= dimenzije) {
          printf("Unijeli ste nepostojece cvorove.\n");
          break;
        }
        while (getchar() != '\n')
          ;
        skiniteGranu(graph, cvor, susjed);
        break;
      case 4:
        if (dimenzije < 0) {
          printf("Niste napravili graf.\n");
          break;
        }
        graph = (ListaSus *)realloc(graph, (dimenzije + 1) * sizeof(ListaSus));
        graph[dimenzije].head = NULL;
        dimenzije += 1;
        printf("Uspjesno ste unijeli cvor u graf.\n");
        break;
      case 5:
        if (dimenzije < 0) {
          printf("Niste napravili graf.\n");
          break;
        }
        printf("Koji cvor zelite da izbrisete: ");
        scanf("%d", &cvorBrisanje);
        if (cvorBrisanje >= dimenzije) {
          printf("Ne postoji taj cvor u grafu.\n");
          break;
        }
        while (getchar() != '\n')
          ;
        brisanjeCvora(&graph, cvorBrisanje, &dimenzije);
        break;
      case 6:
        printaj(dimenzije, graph);
        break;
      case 7:
        if (dimenzije < 0) {
          printf("Niste napravili graf.\n");
          break;
        }
        obrisiGraf(graph, dimenzije);
        graph = NULL;
        printf("Uspjesno ste obrisali graf.");
        break;
      case 8:
        return 0;
      default:
        printf("Nevalidan izbor.\n");
        break;
      }
    }
  }
  return 0;
}