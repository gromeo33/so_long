#ifndef SO_LONG_H
#define SO_LONG_H


#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "./minilibx_macos/mlx.h"
#include "get_next_line.h"

typedef struct s_vars
{
    void *mlx;
    void *win;
    void *img_wall;
    void *img_player;
    void *img_collectible;
    void *img_exit;
    void *img_space;
    char **map;            // Mappa del gioco
    int player_x;          // Posizione X del giocatore
    int player_y;          // Posizione Y del giocatore
    int collectibles;      // Numero di oggetti da raccogliere
} t_vars;


char **read_map(char *map_path);
char **read_map(char *map);
void load_images(t_vars *vars);    // Dichiarazione di load_images
void draw_map(t_vars *vars, char **map); // Dichiarazione di draw_map


#endif

