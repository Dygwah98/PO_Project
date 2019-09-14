#include"../include/level.h"

Level::Level(EventHandler& eh): 
    GameComponent(), graphics(eh.get_graphic_context()), events(eh), lines(), player(), is_stopped(false) {

    Line::setLineDimension(graphics.get_buffer_width());

    for(unsigned i = 0; i < 11; ++i) lines.push_back({});

    cout << "Level::Level() " << this << endl;
}

Level::~Level() {

    cout << "Level::~Level() " << this << endl;
}

Collision Level::player_collides() const { 

    return 
    (contains<Line>(lines, player.get_position()))
    ? lines.at(player.get_position()).check_for_collision(player)
    : GameObject::null_val();
}

bool Level::handle_collisions() {
    //controlla se sono avvenute collisioni sulla linea sulla quale si trova il player
    switch(player_collides()) {
        
        //se la collisione è letale
        case Collision::DEADLY:
            //il player perde una vita
            player.lose_life();
            //se il player arriva a 0 vite: 
            if(player.is_dead()) {
                return true;
            //altrimenti: viene riposizionato all'inizio del livello
            } else {
                //i valori esatti vanno ancora determinati
                player.reposition(0, 0.0f);
            }
                        
        break;
        
        //se il player ha raggiunto il punto d'arrivo
        case Collision::ARRIVAL:
            //il player viene riposizionato, e all'interno di level va registrata la nuova situazione
        break;
        
        //se il player si trova su una piattaforma mobile
        case Collision::LOG:
            //il player viene riposizionato a ogni iterazione, shiftandolo alla velocità della line 
           //player.reposition(..., ...);
        break;

        default:
        break;
    }

    return false;
}

void Level::update_player() {

    //controlla se il player deve muoversi
    //se è stato premuto un tasto: viene specificato al player che inizia il movimento
    if(!player.is_moving() and events.next_key() != Keys::nd/* and player_in_area()*/) {
        
        player.set_dir(events.next_key());
        player.move();
    } 
    //se si sta già movendo: continua (internamente gestirà il cambio del bool isMoving)
    else if(player.is_moving()) 
        player.move();
}

void Level::update_lines() {
    //shifta ogni linea secondo la sua velocità
    for(Line& it : lines)
        it.shift_head();        
}

GameComponent::exec_type Level::exec() {

    GameComponent::exec_type ret = GameComponent::exit_val();

    bool STOP = false;

    events.launch();

    //nella versione finale, da rimuovere
    ALLEGRO_BITMAP* btemp = al_create_bitmap(30, 30);
    //nella versione finale, da rimuovere
    al_set_target_bitmap(btemp);
    //nella versione finale, da rimuovere
    al_clear_to_color(al_map_rgb(255, 255, 255));
    //nella versione finale, da rimuovere
    al_set_target_backbuffer(al_get_current_display());

    while(!STOP) {

        const Event& ev = events.next_event();
        switch(ev) {

            case Event::Execute:
                
                if(!is_stopped) {    
                    if(!player.is_dead()) {
                        STOP = handle_collisions();   
                        update_player();
                    } 
                    update_lines();
                }

            break;

            case Event::Redraw:
                //call the graphic routines
                if(!is_stopped) {
                    //graphics.push_image per ogni componente da rappresentare
                    //nella versione finale, da rimuovere
                    graphics.push_image(btemp, player.get_coord(), player.get_position(), Priority::FRONT, false);
                    //esegue il redraw vero e proprio
                    graphics.redraw();
                }

            break;

            case Event::Exit:        
                //termina il loop di gioco e "chiude" il livello
                STOP = true;

            break;

            case Event::Stop:
                //mette in pausa il gioco
                is_stopped = !is_stopped;
            
            break;
            
            default:
            break;
        }
    }
    
    events.suspend();
    //nella versione finale, da rimuovere
    al_destroy_bitmap(btemp);

    return ret;
}