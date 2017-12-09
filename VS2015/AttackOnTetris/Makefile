 CC=g++
 CFLAGS=-g -Wall
 
 INC=-IX:\GameDev\AttackOnTetris\SDL2-2.0.5\include -IX:\GameDev\AttackOnTetris\SDL2-2.0.5\SDL2_image-2.0.1\include -IX:\GameDev\AttackOnTetris-Dev\SDL2-2.0.5\SDL2_ttf-2.0.14\include
 
 LIB=-LX:\GameDev\AttackOnTetris-Dev\SDL2-2.0.5\lib\x86 -LX:\GameDev\AttackOnTetris-Dev\SDL2-2.0.5\SDL2_image-2.0.1\lib\x86 -LX:\GameDev\AttackOnTetris-Dev\SDL2-2.0.5\SDL2_ttf-2.0.14\lib\x86
 
 all: AttackOnTetrisGpp
 
 AttackOnTetrisGpp: main.o Animation.o Animator.o AnimatorPanel.o AnimatorSwapper.o Entity.o EntityManager.o FontManager.o Game.o Input.o InputController.o MainMenu.o Panel.o RenderSystem.o SceneMainMenu.o SDL_FontCache.c Sprite.o Swapper.o TextureManager.o UIElement.o UIManager.o UIText.o Vector2.o
	g++ main.o Animation.o Animator.o AnimatorPanel.o AnimatorSwapper.o Entity.o EntityManager.o FontManager.o Game.o Input.o InputController.o MainMenu.o Panel.o RenderSystem.o SceneMainMenu.o SDL_FontCache.c Sprite.o Swapper.o TextureManager.o UIElement.o UIManager.o UIText.o Vector2.o -o AttackOnTetrisGpp
	
main.o: main.cpp
	g++ $(CFLAGS) $(INC) $(LIB) -c main.cpp
	
Animation.o: Animation.cpp
	g++ $(CFLAGS) $(INC) $(LIB) -c Animation.cpp
	
Animator.o: Animator.cpp
	g++ $(CFLAGS) $(INC) $(LIB) -c Animator.cpp
	
AnimatorPanel.o: AnimatorPanel.cpp
	g++ $(CFLAGS) $(INC) $(LIB) -c AnimatorPanel.cpp
	
AnimatorSwapper.o: AnimatorSwapper.cpp
	g++ $(CFLAGS) $(INC) $(LIB) -c AnimatorSwapper.cpp
	
Entity.o: Entity.cpp
	g++ $(CFLAGS) $(INC) $(LIB) -c Entity.cpp
	
EntityManager.o: EntityManager.cpp
	g++ $(CFLAGS) $(INC) $(LIB) -c EntityManager.cpp
	
FontManager.o: FontManager.cpp
	g++ $(CFLAGS) $(INC) $(LIB) -c FontManager.cpp
	
Game.o: Game.cpp
	g++ $(CFLAGS) $(INC) $(LIB) -c Game.cpp
	
Input.o: Input.cpp
	g++ $(CFLAGS) $(INC) $(LIB) -c Input.cpp
	
InputController.o: InputController.cpp
	g++ $(CFLAGS) $(INC) $(LIB) -c InputController.cpp
	
MainMenu.o: MainMenu.cpp
	g++ $(CFLAGS) $(INC) $(LIB) -c MainMenu.cpp
	
Panel.o: Panel.cpp
	g++ $(CFLAGS) $(INC) $(LIB) -c Panel.cpp
	
RenderSystem.o: RenderSystem.cpp
	g++ $(CFLAGS) $(INC) $(LIB) -c RenderSystem.cpp
	
SceneMainMenu.o: SceneMainMenu.cpp
	g++ $(CFLAGS) $(INC) $(LIB) -c SceneMainMenu.cpp
	
SDL_FontCache.o: SDL_FontCache.c
	gcc $(CFLAGS) $(INC) $(LIB) -c SDL_FontCache.c
	
Sprite.o: Sprite.cpp
	g++ $(CFLAGS) $(INC) $(LIB) -c Sprite.cpp
	
Swapper.o: Swapper.cpp
	g++ $(CFLAGS) $(INC) $(LIB) -c Swapper.cpp
	
TextureManager.o: TextureManager.cpp
	g++ $(CFLAGS) $(INC) $(LIB) -c TextureManager.cpp
	
UIElement.o: UIElement.cpp
	g++ $(CFLAGS) $(INC) $(LIB) -c UIElement.cpp
	
UIManager.o: UIManager.cpp
	g++ $(CFLAGS) $(INC) $(LIB) -c UIManager.cpp
	
UIText.o: UIText.cpp
	g++ $(CFLAGS) $(INC) $(LIB) -c UIText.cpp
	
Vector2.o: Vector2.cpp
	g++ $(CFLAGS) $(INC) $(LIB) -c Vector2.cpp
	
clean:
	rm *o AttackOnTetrisGpp