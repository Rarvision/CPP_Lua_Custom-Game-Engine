#include "System.hpp"

class SDLGraphicsSystem : public ISystem{
    public:
    
        /** Destructor 
         *
         */
        ~SDLGraphicsSystem();

        void SetSDLWindow(int w, int h);

        void SetRenderDrawColor(int r, int g, int b, int a);
        /**
         * Clear the screen
         */
        void RenderClear();
        /**
         * Render whatever is in the backbuffer to
         * the screen.
         */
        void RenderPresent();
        /**
         * Get Pointer to Window
         */
        SDL_Window* GetWindow();
        /**
         * Get Pointer to Renderer
         */
        SDL_Renderer* GetSDLRenderer();

        void Init() override;

        void Update() override;

        void Clear() override;

private:
        int             mScreenWidth{0};
        int             mScreenHeight{0};
        // SDL Window
        SDL_Window*     mWindow = nullptr;
        // SDL Renderer
        SDL_Renderer*   mRenderer = nullptr;
};