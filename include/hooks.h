#ifndef HOOKS
    #define HOOKS

    namespace GameHook
    {
        void Set(const BYTE*, const std::vector<BYTE>&);

        extern "C"
        {
        }

        bool CreateHitStreakHook();
        void CreateGameHooks();
    }
#endif