#include <stdio.h>
#include <stdlib.h>
#include "maze.h"
#include "svg.h" 
#include <time.h>

tree *create_node(rectangle area){
    tree *node = malloc(sizeof(tree));

    if (node == NULL){ return NULL;}

    node->rectangle = area;
    node->wall_direction = HORIZONTAL;
    node->wall_pos = 0;
    node->door_pos = 0;

    node->left = NULL;
    node->right = NULL;

    return node;

}



int can_divide(rectangle area){
    if (area.height <= 1 || area.width <= 1){
        return 0; // can't
    } 
    return 1; // can
}

int random_between(int a, int b)
{
    return a + rand() % (b - a + 1);
}


void split_region(tree *node, rectangle *rectangle_left,  rectangle *rectangle_right ){
    
    rectangle area = node->rectangle;

    if (area.width >= area.height){
        node->wall_direction = VERTICAL;

        int distance = random_between(1,area.width - 1);

        // index wall
        node->wall_pos = area.x + distance;

        // random door pos in wall
        node->door_pos = random_between(area.y, area.y + area.height - 1);

        // left rectangle 
        rectangle_left->x = area.x;
        rectangle_left->y = area.y;
        rectangle_left->width = distance;
        rectangle_left->height = area.height;

        // right rectangle
        rectangle_right->x = area.x + distance;
        rectangle_right->y = area.y;
        rectangle_right->width = area.width - distance;
        rectangle_right->height = area.height;
    }else {

        node->wall_direction = HORIZONTAL;

        // random wall pos
        int distance = random_between(1, area.height - 1);

         // index wall 
        node->wall_pos = area.y + distance;
        
        // random door pos in wall
        node->door_pos = random_between(area.x, area.x + area.width - 1);

        // top rectangle
        rectangle_left->x = area.x;
        rectangle_left->y = area.y;
        rectangle_left->width = area.width;
        rectangle_left->height = distance;

        // bottom rectangle
        /* Rectangle du bas */
        rectangle_right->x = area.x;
        rectangle_right->y = area.y + distance;
        rectangle_right->width = area.width;
        rectangle_right->height = area.height - distance;
    }
}



tree *generate_region(rectangle area){

    tree *node = create_node(area);
    if (node == NULL)
        return NULL;

    if (can_divide(area) == 0 ){
        // just une feuille
        return node;
    }

    rectangle rectangle_left;
    rectangle rectangle_right;

    split_region(node, &rectangle_left, &rectangle_right);

    node->left = generate_region(rectangle_left);
    node->right = generate_region(rectangle_right);

    return node;


}

maze *maze_random(int width,int height){

    srand(time(NULL));
    rectangle init;
    init.x = 0;
    init.y = 0;
    init.width = width;
    init.height = height;
    return generate_region(init);
}

void draw_tree(FILE *f, tree *node){
    if (node == NULL || (node->left == NULL && node->right == NULL))
        return;

    rectangle area = node->rectangle;

    if (node->wall_direction == VERTICAL){

        svg_line(f,node->wall_pos,area.y,node->wall_pos,node->door_pos);
        svg_line(f,node->wall_pos, node->door_pos+1,node->wall_pos,area.y+area.height);
    } else {
        svg_line(f,area.x,node->wall_pos,node->door_pos,node->wall_pos);
        svg_line(f,node->door_pos + 1,node->wall_pos,area.x + area.width,node->wall_pos);
    }
    draw_tree(f, node->left);
    draw_tree(f, node->right);

}





void maze_svg(maze *m, char *filename)
{
    FILE *f = fopen(filename, "w");

    if (f == NULL) {
        return;
    }

    unsigned int width = m->rectangle.width;
    unsigned int height = m->rectangle.height;


    svg_header(f, width,height);

    // drax the contour
    svg_rect(f, 0, 0, width, height);

    // draw all the inside wall
    draw_tree(f, m);

    svg_footer(f);
    fclose(f);
}