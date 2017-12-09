cmake_minimum_required (VERSION 2.9)
project (AttackOnPanel)
 
add_definitions("-static-libgcc -static-libstdc++") 
 
# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/include" "${PROJECT_SOURCE_DIR}/SDL2-2.0.5/include" "${PROJECT_SOURCE_DIR}/SDL2-2.0.5/SDL2_image-2.0.1/include" "${PROJECT_SOURCE_DIR}/SDL2-2.0.5/SDL2_mixer-2.0.1/include" "${PROJECT_SOURCE_DIR}/SDL2-2.0.5/SDL2_ttf-2.0.14/include")
 
# add the executable
add_executable(AttackOnPanel Vector2.cpp UIText.cpp UIManager.cpp TimeAttackTimer.cpp TextureManager.cpp Swapper.cpp Sprite.cpp AudioManager.cpp ScreenFader.cpp ScreenFade.cpp RenderSystem.cpp PauseMenu.cpp AnimatorPanel.cpp Panel.cpp MainMenu.cpp main.cpp LogSpiralParticle.cpp IRenderable.cpp InputController.cpp Input.cpp GameTimer.cpp GameManager.cpp GameGrid.cpp Game.cpp FontManager.cpp EntityManager.cpp Entity.cpp EndlessTimer.cpp DigitPool.cpp Digit.cpp ComboSign.cpp ColorCode.cpp ChainSign.cpp AnimatorSwapper.cpp Animator.cpp Animation.cpp SDL_FontCache.c)

target_link_libraries(AttackOnPanel "${PROJECT_SOURCE_DIR}/SDL2-2.0.5/lib/x86/SDL2main.lib" "${PROJECT_SOURCE_DIR}/SDL2-2.0.5/lib/x86/SDL2.lib" "${PROJECT_SOURCE_DIR}/SDL2-2.0.5/SDL2_image-2.0.1/lib/x86/SDL2_image.lib" "${PROJECT_SOURCE_DIR}/SDL2-2.0.5/SDL2_mixer-2.0.1/lib/x86/SDL2_mixer.lib" "${PROJECT_SOURCE_DIR}/SDL2-2.0.5/SDL2_ttf-2.0.14/lib/x86/SDL2_ttf.lib")