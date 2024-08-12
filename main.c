#include "so_long.h"
#include "./minilibx_macos/mlx.h"
#include "get_next_line.h"
#include <string.h>

char **read_map(char *map_path)
{
    int fd;
    char **map;
    char *line;
    int i = 0;

    // Apri il file in lettura
    fd = open(map_path, O_RDONLY);
    if (fd < 0)
    {
        printf("Error: Unable to open map file.\n");
        return (NULL);
    }

    // Allocazione di memoria per la mappa
    map = malloc(sizeof(char *) * 100); // Supponendo massimo 100 righe
    if (!map)
        return (NULL);

    // Legge il file linea per linea
    while ((line = get_next_line(fd)))
    {
        map[i] = line;
        i++;
    }
    map[i] = NULL; // Termina la mappa con NULL
    close(fd); // Chiudi il file
    return (map);
}

// Funzione per caricare le immagini
void load_images(t_vars *vars)
{
    int img_width = 896;
    int img_height = 320;

    vars->img_wall = mlx_xpm_file_to_image(vars->mlx, "wall.xpm", &img_width, &img_height);
    if (!vars->img_wall)
    {
        printf("Error loading wall.xpm\n");
        exit(1);
    }

    vars->img_player = mlx_xpm_file_to_image(vars->mlx, "player.xpm", &img_width, &img_height);
    if (!vars->img_player)
    {
        printf("Error loading player.xpm\n");
        exit(1);
    }

    vars->img_collectible = mlx_xpm_file_to_image(vars->mlx, "collectible.xpm", &img_width, &img_height);
    if (!vars->img_collectible)
    {
        printf("Error loading collectible.xpm\n");
        exit(1);
    }

    vars->img_exit = mlx_xpm_file_to_image(vars->mlx, "exit.xpm", &img_width, &img_height);
    if (!vars->img_exit)
    {
        printf("Error loading exit.xpm\n");
        exit(1);
    }

    vars->img_space = mlx_xpm_file_to_image(vars->mlx, "space.xpm", &img_width, &img_height);
    if (!vars->img_space)
    {
        printf("Error loading space.xpm\n");
        exit(1);
    }
}
// funzione per raccogliere gli oggetti
int count_collectibles(char **map)
{
    int x, y;
    int count = 0;

    y = 0;
    while (map[y])
    {
        x = 0;
        while (map[y][x])
        {
            if (map[y][x] == 'C')
                count++;
            x++;
        }
        y++;
    }
    return count;
}
//funzione per trovare la posizione corretta del giocatore
int find_player_x(char **map)
{
    int x, y;

    y = 0;
    while (map[y])
    {
        x = 0;
        while (map[y][x])
        {
            if (map[y][x] == 'P')
                return x;
            x++;
        }
        y++;
    }
    return -1; // Non trovato
}

int find_player_y(char **map)
{
    int x, y;

    y = 0;
    while (map[y])
    {
        x = 0;
        while (map[y][x])
        {
            if (map[y][x] == 'P')
                return y;
            x++;
        }
        y++;
    }
    return -1; // Non trovato
}
//funzione per il movimento del giocatore
void move_player(t_vars *vars, int dx, int dy)
{
    int new_x = vars->player_x + dx;
    int new_y = vars->player_y + dy;

    // Verifica che il movimento sia valido (non colpire i muri)
    if (vars->map[new_y][new_x] != '1')
    {
        // Se c'è un oggetto, raccoglilo
        if (vars->map[new_y][new_x] == 'C')
        {
            vars->collectibles--;
            vars->map[new_y][new_x] = '0'; // Rimuove l'oggetto dalla mappa
        }

        // Controlla se il giocatore raggiunge l'uscita
        if (vars->map[new_y][new_x] == 'E' && vars->collectibles == 0)
        {
            printf("You Win!\n");
            mlx_destroy_window(vars->mlx, vars->win);
            exit(0);
        }

        // Aggiorna la posizione del giocatore
        vars->map[vars->player_y][vars->player_x] = '0'; // Vecchia posizione
        vars->player_x = new_x;
        vars->player_y = new_y;
        vars->map[vars->player_y][vars->player_x] = 'P'; // Nuova posizione

        // Ridisegna la mappa
        draw_map(vars, vars->map);
    }
}

//funzione per gestire i movimenti da tastiera
int handle_keypress(int keycode, t_vars *vars)
{
    if (keycode == 53) // Tasto ESC per uscire
    {
        mlx_destroy_window(vars->mlx, vars->win);
        exit(0);
    }
    else if (keycode == 0) // A per muovere a sinistra
        move_player(vars, -1, 0);
    else if (keycode == 1) // S per muovere giù
        move_player(vars, 0, 1);
    else if (keycode == 2) // D per muovere a destra
        move_player(vars, 1, 0);
    else if (keycode == 13) // W per muovere su
        move_player(vars, 0, -1);

    return (0);
}



// Funzione per disegnare la mappa
void draw_map(t_vars *vars, char **map)
{
    int x, y;

    y = 0;
    while (map[y])
    {
        x = 0;
        while (map[y][x])
        {
            if (map[y][x] == '1')
                mlx_put_image_to_window(vars->mlx, vars->win, vars->img_wall, x * 64, y * 64);
            else if (map[y][x] == 'P')
                mlx_put_image_to_window(vars->mlx, vars->win, vars->img_player, x * 64, y * 64);
            else if (map[y][x] == 'C')
                mlx_put_image_to_window(vars->mlx, vars->win, vars->img_collectible, x * 64, y * 64);
            else if (map[y][x] == 'E')
                mlx_put_image_to_window(vars->mlx, vars->win, vars->img_exit, x * 64, y * 64);
            else
                mlx_put_image_to_window(vars->mlx, vars->win, vars->img_space, x * 64, y * 64);
            x++;
        }
        y++;
    }
}


//controlla la grandezza della mappa
int get_map_width(char **map)
{
    return (strlen(map[0]));  // Presume che tutte le righe abbiano la stessa lunghezza
}

int get_map_height(char **map)
{
    int height = 0;

    while (map[height])
        height++;
    return height;
}


// Funzione principale
int main(int argc, char **argv)
{
    t_vars vars;
    char **map;
    int map_width;
    int map_height;

    if (argc != 2)
    {
        printf("Error\nInvalid number of arguments\n");
        return (1);
    }

    map = read_map(argv[1]);
    if (!map)
    {
        printf("Error\nFailed to read map\n");
        return (1);
    }

    vars.map = map;
    vars.collectibles = count_collectibles(map);
    vars.player_x = find_player_x(map);
    vars.player_y = find_player_y(map);

    // Calcola la larghezza della mappa basandosi sulla prima riga
    map_width = strlen(map[0]);  // Controlla la lunghezza della prima riga
    // Calcola l'altezza della mappa contando il numero di righe
    map_height = 0;
    while (map[map_height])
        map_height++;

    vars.mlx = mlx_init();

    // Crea la finestra esattamente in base alle dimensioni calcolate
    vars.win = mlx_new_window(vars.mlx, map_width * 64, map_height * 64, "so_long");

    load_images(&vars);
    draw_map(&vars, map);

    // Gestione degli eventi di tastiera
    mlx_hook(vars.win, 2, 1L<<0, handle_keypress, &vars);

    mlx_loop(vars.mlx);
    return (0);
}


