#ifndef HOOKS
    #define HOOKS

    namespace GameHook
    {
        void Set(const BYTE*, const std::vector<BYTE>&);

        extern "C"
        {
        }

        void CreateHitStreakHook();
        void CreateGameHooks();
    }
#endif