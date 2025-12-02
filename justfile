# Define a recipe to run the Python script with the day parameter
new-day day:
    @echo "Creating new day {{day}}..."
    uv run util/new-day.py {{day}}
    
build day:
    @echo "Building day {{day}}..."
    cmake --build build --target day-{{day}}

test day:
    @echo "Running tests for day {{day}}..."
    ctest --test-dir build --output-on-failure -R day-{{day}}

clean:
    @echo "Cleaning build directory..."
    rm -rf build/*

make-build:
    @echo "Configuring build directory..."
    cmake -S . -B build