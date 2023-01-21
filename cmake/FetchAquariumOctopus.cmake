include(FetchContent)

FetchContent_Declare(
        aquarium-octopus
        GIT_REPOSITORY https://github.com/pretore/aquarium-octopus.git
        GIT_TAG v1.0.0
        GIT_SHALLOW 1
)

FetchContent_MakeAvailable(aquarium-octopus)
