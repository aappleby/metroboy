const char* terminus =
".......|.......|.......|.......|.......|.......|.......|.......|######.|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|"
".......|.......|.......|.......|.......|.......|.......|.......|######.|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|"
".......|.####..|.####..|.......|.......|..#....|..#....|.......|######.|.......|.......|..####.|.###...|.......|.####..|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|#######|"
".......|#....#.|######.|.#.#...|..#....|.###...|..#....|.......|######.|.......|.......|....##.|#...#..|.......|.#..#..|..#....|###....|###....|###....|###....|###....|###....|###....|###....|###....|###....|###....|###....|.......|.......|.......|#######|"
".......|##..##.|#.##.#.|#####..|.###...|..#....|.###...|..##...|##..##.|..##...|.......|...#.#.|#...#..|.......|.####..|#.#.#..|#.#....|#.#....|#.#....|#.#....|#.#....|#.#....|#.#....|#.#....|#.#....|#.#....|#.#....|#.#....|.......|.......|.......|#######|"
".......|#....#.|######.|#####..|#####..|#.#.#..|#####..|.####..|#....#.|.#..#..|.......|.###...|#...#..|.......|.#..#..|.###...|#.#....|#.#....|#.#....|#.#....|#.#....|#.#....|#.#....|#.#....|#.#....|#.#....|#.#....|#.#....|.......|.......|.......|#######|"
".......|#.##.#.|#....#.|#####..|.###...|#####..|#####..|.####..|#....#.|.#..#..|.......|#...#..|.###...|.......|.#..#..|##.##..|#.#....|#.#....|#.#....|#.#....|#.#....|#.#....|#.#....|#.#....|#.#....|#.#....|#.#....|#.#....|.......|.......|...#...|###.###|"
".......|#.##.#.|##..##.|.###...|..#....|#.#.#..|.###...|..##...|##..##.|..##...|.......|#...#..|..#....|.......|.#..#..|.###...|#.#....|#.#....|#.#....|#.#....|#.#....|#.#....|#.#....|#.#....|#.#....|#.#....|#.#....|#.#....|.......|.......|.......|#######|"
".......|#....#.|######.|.###...|.......|..#....|..#....|.......|######.|.......|.......|#...#..|#####..|.......|.#..#..|#.#.#..|#.#....|#.#....|#.#....|#.#....|#.#....|#.#....|#.#....|#.#....|#.#....|#.#....|#.#....|#.#....|.......|.......|.......|#######|"
".......|.####..|.####..|..#....|.......|.###...|.###...|.......|######.|.......|.......|.###...|..#....|.......|.#.#...|..#....|###....|###....|###....|###....|###....|###....|###....|###....|###....|###....|###....|###....|.......|.......|.......|#######|"
".......|.......|.......|.......|.......|.......|.......|.......|######.|.......|.......|.......|.......|.......|#......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|#######|#######|"
".......|.......|.......|.......|.......|.......|.......|.......|######.|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|"
".......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|"
".......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|"
".......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|"
".......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|"

".......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|"
".......|.......|.#.#...|.......|.......|.......|.......|..#....|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|"
".......|..#....|.#.#...|.#.#...|..#....|.#..#..|..#....|..#....|...#...|.#.....|.......|.......|.......|.......|.......|....#..|.###...|..#....|.###...|.###...|....#..|#####..|.###...|#####..|.###...|.###...|.......|.......|.......|.......|.......|.###...|"
".......|..#....|.#.#...|.#.#...|.###...|#.#.#..|.#.#...|..#....|..#....|..#....|.......|.......|.......|.......|.......|....#..|#...#..|.##....|#...#..|#...#..|...##..|#......|#......|....#..|#...#..|#...#..|.......|.......|....#..|.......|.#.....|#...#..|"
".......|..#....|.......|#####..|#.#.#..|.#.#...|.#.#...|.......|.#.....|...#...|.#.#...|..#....|.......|.......|.......|...#...|#..##..|..#....|#...#..|....#..|..#.#..|#......|#......|....#..|#...#..|#...#..|..#....|..#....|...#...|#####..|..#....|#...#..|"
".......|..#....|.......|.#.#...|#.#....|...#...|..#....|.......|.#.....|...#...|..#....|..#....|.......|.......|.......|...#...|#.#.#..|..#....|...#...|..##...|.#..#..|####...|####...|...#...|.###...|#...#..|..#....|..#....|..#....|.......|...#...|...#...|"
".......|..#....|.......|.#.#...|.###...|..#....|.##.#..|.......|.#.....|...#...|#####..|#####..|.......|#####..|.......|..#....|##..#..|..#....|..#....|....#..|#...#..|....#..|#...#..|...#...|#...#..|.####..|.......|.......|.#.....|.......|....#..|..#....|"
".......|.......|.......|#####..|..#.#..|..#.#..|#..#...|.......|.#.....|...#...|..#....|..#....|.......|.......|.......|..#....|#...#..|..#....|.#.....|....#..|#####..|....#..|#...#..|..#....|#...#..|....#..|.......|.......|..#....|#####..|...#...|.......|"
".......|..#....|.......|.#.#...|#.#.#..|.#.#.#.|#..#...|.......|..#....|..#....|.#.#...|..#....|..#....|.......|..#....|.#.....|#...#..|..#....|#......|#...#..|....#..|#...#..|#...#..|..#....|#...#..|....#..|..#....|..#....|...#...|.......|..#....|..#....|"
".......|..#....|.......|.#.#...|.###...|.#..#..|.##.#..|.......|...#...|.#.....|.......|.......|..#....|.......|..#....|.#.....|.###...|.###...|#####..|.###...|....#..|.###...|.###...|..#....|.###...|.###...|..#....|..#....|....#..|.......|.#.....|..#....|"
".......|.......|.......|.......|..#....|.......|.......|.......|.......|.......|.......|.......|.#.....|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.#.....|.......|.......|.......|.......|"
".......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|"
".......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|"
".......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|"
".......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|"
".......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|"

".......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|"
".......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|..#....|.......|"
".###...|.###...|####...|.###...|###....|#####..|#####..|.###...|#...#..|.###...|..###..|#...#..|#......|#...#..|#...#..|.###...|####...|.###...|####...|.###...|#####..|#...#..|#...#..|#...#..|#...#..|#...#..|#####..|.###...|.#.....|.###...|.#.#...|.......|"
"#...#..|#...#..|#...#..|#...#..|#..#...|#......|#......|#...#..|#...#..|..#....|...#...|#..#...|#......|##.##..|#...#..|#...#..|#...#..|#...#..|#...#..|#...#..|..#....|#...#..|#...#..|#...#..|#...#..|#...#..|....#..|.#.....|.#.....|...#...|#...#..|.......|"
"#..##..|#...#..|#...#..|#......|#...#..|#......|#......|#......|#...#..|..#....|...#...|#.#....|#......|#.#.#..|##..#..|#...#..|#...#..|#...#..|#...#..|#......|..#....|#...#..|#...#..|#...#..|.#.#...|.#.#...|...#...|.#.....|..#....|...#...|.......|.......|"
"#.#.#..|#...#..|####...|#......|#...#..|####...|####...|#......|#####..|..#....|...#...|##.....|#......|#.#.#..|#.#.#..|#...#..|#...#..|#...#..|#...#..|.###...|..#....|#...#..|.#.#...|#...#..|..#....|.#.#...|..#....|.#.....|..#....|...#...|.......|.......|"
"#.#.#..|#####..|#...#..|#......|#...#..|#......|#......|#.###..|#...#..|..#....|...#...|##.....|#......|#...#..|#..##..|#...#..|####...|#...#..|####...|....#..|..#....|#...#..|.#.#...|#.#.#..|..#....|..#....|.#.....|.#.....|...#...|...#...|.......|.......|"
"#..##..|#...#..|#...#..|#......|#...#..|#......|#......|#...#..|#...#..|..#....|#..#...|#.#....|#......|#...#..|#...#..|#...#..|#......|#...#..|#.#....|....#..|..#....|#...#..|.#.#...|#.#.#..|.#.#...|..#....|#......|.#.....|...#...|...#...|.......|.......|"
"#......|#...#..|#...#..|#...#..|#..#...|#......|#......|#...#..|#...#..|..#....|#..#...|#..#...|#......|#...#..|#...#..|#...#..|#......|#.#.#..|#..#...|#...#..|..#....|#...#..|..#....|##.##..|#...#..|..#....|#......|.#.....|....#..|...#...|.......|.......|"
".####..|#...#..|####...|.###...|###....|#####..|#......|.###...|#...#..|.###...|.##....|#...#..|#####..|#...#..|#...#..|.###...|#......|.###...|#...#..|.###...|..#....|.###...|..#....|#...#..|#...#..|..#....|#####..|.###...|....#..|.###...|.......|.......|"
".......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|....#..|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|#####..|"
".......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|"
".......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|"
".......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|"
".......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|"
".......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|"

".#.....|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|"
"..#....|.......|.......|.......|.......|.......|.......|.......|.......|..#....|....#..|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.#..#..|.......|"
".......|.......|#......|.......|....#..|.......|...##..|.......|#......|..#....|....#..|.#.....|.##....|.......|.......|.......|.......|.......|.......|.......|..#....|.......|.......|.......|.......|.......|.......|...##..|..#....|.##....|#.#.#..|.......|"
".......|.......|#......|.......|....#..|.......|..#....|.......|#......|.......|.......|.#.....|..#....|.......|.......|.......|.......|.......|.......|.......|..#....|.......|.......|.......|.......|.......|.......|..#....|..#....|...#...|#..#...|.......|"
".......|.###...|####...|.###...|.####..|.###...|.###...|.####..|####...|.##....|...##..|.#..#..|..#....|####...|####...|.###...|####...|.####..|#.###..|.####..|.###...|#...#..|#...#..|#...#..|#...#..|#...#..|#####..|..#....|..#....|...#...|.......|..#....|"
".......|....#..|#...#..|#...#..|#...#..|#...#..|..#....|#...#..|#...#..|..#....|....#..|.#.#...|..#....|#.#.#..|#...#..|#...#..|#...#..|#...#..|##.....|#......|..#....|#...#..|#...#..|#...#..|.#.#...|#...#..|...#...|.#.....|..#....|....#..|.......|.#.#...|"
".......|.####..|#...#..|#......|#...#..|#####..|..#....|#...#..|#...#..|..#....|....#..|.##....|..#....|#.#.#..|#...#..|#...#..|#...#..|#...#..|#......|.###...|..#....|#...#..|.#.#...|#.#.#..|..#....|#...#..|..#....|..#....|..#....|...#...|.......|#...#..|"
".......|#...#..|#...#..|#......|#...#..|#......|..#....|#...#..|#...#..|..#....|....#..|.##....|..#....|#.#.#..|#...#..|#...#..|#...#..|#...#..|#......|....#..|..#....|#...#..|.#.#...|#.#.#..|..#....|#...#..|.#.....|..#....|..#....|...#...|.......|#...#..|"
".......|#...#..|#...#..|#...#..|#...#..|#...#..|..#....|#...#..|#...#..|..#....|....#..|.#.#...|..#....|#.#.#..|#...#..|#...#..|#...#..|#...#..|#......|....#..|..#....|#...#..|..#....|#.#.#..|.#.#...|#...#..|#......|..#....|..#....|...#...|.......|#...#..|"
".......|.####..|####...|.###...|.####..|.###...|..#....|.####..|#...#..|.###...|....#..|.#..#..|.###...|#.#.#..|#...#..|.###...|####...|.####..|#......|####...|...##..|.####..|..#....|.###...|#...#..|.####..|#####..|...##..|..#....|.##....|.......|#####..|"
".......|.......|.......|.......|.......|.......|.......|....#..|.......|.......|.#..#..|.......|.......|.......|.......|.......|#......|....#..|.......|.......|.......|.......|.......|.......|.......|....#..|.......|.......|.......|.......|.......|.......|"
".......|.......|.......|.......|.......|.......|.......|.###...|.......|.......|..##...|.......|.......|.......|.......|.......|#......|....#..|.......|.......|.......|.......|.......|.......|.......|.###...|.......|.......|.......|.......|.......|.......|"
".......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|"
".......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|"
".......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|"
".......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|"

".......|.......|.......|.......|.......|.......|.......|.......|..#....|.......|.#.#...|.......|.......|.......|.#.#...|.......|.......|.......|.......|.......|.......|.......|.......|.......|..#.#..|.......|.......|.......|.......|.......|.......|.#.#...|"
".......|.......|.......|.......|.......|.......|.......|.......|.#.#...|.......|..#....|.......|.......|.......|..#....|.......|.......|...#...|..#....|..#.#..|..#.#..|.......|.......|.......|.#.#...|.......|.......|.......|.......|.......|.......|.#.#...|"
".......|.......|.......|...#...|.......|.......|..#....|..#....|.......|.#...#.|.###...|.......|.#####.|.......|#####..|.......|.......|..#....|..#....|.#.#...|..#.#..|.......|.......|.......|.......|####.#.|.#.#...|.......|.......|.......|.#.#...|#...#..|"
"..###..|.......|.......|..#.#..|.......|.......|.###...|.###...|.......|#.#.#..|#...#..|....#..|#..#...|.......|....#..|.......|.......|..#....|.#.....|.#.#...|.#.#...|.......|.......|.......|.......|.#.###.|..#....|.#.....|.......|.......|..#....|#...#..|"
".#...#.|.......|.......|..#....|.......|.......|..#....|..#....|.......|.#.#...|#......|...#...|#..#...|.......|...#...|.......|.......|.......|.......|.......|.......|..##...|.......|.......|.......|.#.#.#.|.####..|..#....|.###...|.......|#####..|.#.#...|"
"####...|.......|.......|.###...|.......|.......|..#....|..#....|.......|..#....|.###...|..#....|#..###.|.......|..#....|.......|.......|.......|.......|.......|.......|.####..|.......|.......|.......|.#.#.#.|#......|...#...|#.#.#..|.......|...#...|.#.#...|"
".#.....|.......|.......|..#....|.......|.......|..#....|..#....|.......|.#.....|....#..|.#.....|#..#...|.......|.#.....|.......|.......|.......|.......|.......|.......|.####..|#####..|#####..|.......|.......|.###...|....#..|#.#.#..|.......|..#....|..#....|"
"####...|.......|.......|..#....|.......|.......|..#....|..#....|.......|#.#.#..|....#..|..#....|#..#...|.......|#......|.......|.......|.......|.......|.......|.......|..##...|.......|.......|.......|.......|....#..|...#...|#.##...|.......|.#.....|..#....|"
".#...#.|.......|..#....|..#....|.#.#...|#.#.#..|..#....|.###...|.......|.#.#.#.|#...#..|...#...|#..#...|.......|#......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|....#..|..#....|#.#....|.......|#......|..#....|"
"..###..|.......|..#....|..#....|.#.#...|#.#.#..|..#....|..#....|.......|..#.#..|.###...|....#..|.#####.|.......|#####..|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|####...|.#.....|.####..|.......|#####..|..#....|"
".......|.......|.#.....|#.#....|#.#....|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|"
".......|.......|.......|.#.....|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|"
".......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|"
".......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|"
".......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|"
".......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|"

".......|.......|.......|.......|.......|.......|.......|.......|.#.#...|.......|..##...|.......|.......|.......|.......|.###...|.......|.......|.......|.......|...#...|.......|.......|.......|.......|.......|..##...|.......|.#.....|.#.....|##.....|.......|"
".......|.......|.......|.......|.......|.......|.......|..##...|.#.#...|.......|....#..|.......|.......|.......|.......|.......|..#....|.......|..##...|.###...|..#....|.......|.......|.......|.......|..#....|.#..#..|.......|##.....|##.....|..#....|.......|"
".......|..#....|.......|..##...|.......|#...#..|..#....|.#..#..|.......|.####..|..###..|.......|.......|.......|.####..|.......|.#.#...|.......|.#..#..|....#..|.......|.......|.####..|.......|.......|.##....|.#..#..|.......|.#.....|.#...#.|##.....|..#....|"
".......|..#....|..#....|.#..#..|.#..#..|#...#..|..#....|..#....|.......|#....#.|.#..#..|...#.#.|.......|.......|#....#.|.......|..#....|..#....|...#...|..##...|.......|.......|#.#.#..|.......|.......|..#....|.#..#..|#.#....|.#..#..|.#..#..|..#..#.|..#....|"
".......|.......|.###...|.#.....|..##...|.#.#...|..#....|.#.#...|.......|#.##.#.|..###..|..#.#..|#####..|.......|#.##.#.|.......|.......|..#....|..#....|....#..|.......|#...#..|#.#.#..|.......|.......|..#....|..##...|.#.#...|...#...|...#...|##..#..|.......|"
".......|..#....|#.#.#..|####...|.#..#..|..#....|.......|.#..#..|.......|#.#..#.|.......|.#.#...|....#..|.......|#.#.##.|.......|.......|#####..|.####..|.###...|.......|#...#..|#.#.#..|..#....|.......|.......|.......|..#.#..|..#....|..#....|...#...|..#....|"
".......|..#....|#.#....|.#.....|.#..#..|.###...|.......|..#.#..|.......|#.#..#.|.####..|#.#....|....#..|.####..|#.##.#.|.......|.......|..#....|.......|.......|.......|#...#..|.##.#..|..#....|.......|.......|.####..|...#.#.|.#..#..|.#.....|..#..#.|.#.....|"
".......|..#....|#.#....|.#.....|..##...|..#....|..#....|...#...|.......|#.##.#.|.......|.#.#...|....#..|.......|#.#.##.|.......|.......|..#....|.......|.......|.......|#...#..|..#.#..|.......|.......|.......|.......|..#.#..|#..##..|#..##..|.#..##.|#...#..|"
".......|..#....|#.#.#..|.#..#..|.#..#..|.###...|..#....|.#..#..|.......|#....#.|.......|..#.#..|.......|.......|#....#.|.......|.......|.......|.......|.......|.......|#..##..|..#.#..|.......|.......|.......|.......|.#.#...|..#.#..|..#..#.|#..#.#.|#...#..|"
".......|..#....|.###...|#####..|.......|..#....|..#....|..##...|.......|.####..|.......|...#.#.|.......|.......|.####..|.......|.......|#####..|.......|.......|.......|###.#..|..#.#..|.......|..#....|.......|.......|#.#....|.####..|....#..|..####.|.###...|"
".......|.......|..#....|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|#......|.......|.......|..#....|.......|.......|.......|....#..|...#...|.....#.|.......|"
".......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|#......|.......|.......|.#.....|.......|.......|.......|....#..|..####.|.....#.|.......|"
".......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|"
".......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|"
".......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|"
".......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|"

".#.....|...#...|..#....|..#.#..|.#.#...|..#....|.......|.......|.#.....|...#...|..#....|.#.#...|.#.....|...#...|..#....|.#.#...|.......|..#.#..|.#.....|...#...|..#....|..#.#..|.#.#...|.......|.......|.#.....|...#...|..#....|.#.#...|...#...|.......|.......|"
"..#....|..#....|.#.#...|.#.#...|.#.#...|.#.#...|.......|.......|..#....|..#....|.#.#...|.#.#...|..#....|..#....|.#.#...|.#.#...|.......|.#.#...|..#....|..#....|.#.#...|.#.#...|.#.#...|.......|.......|..#....|..#....|.#.#...|.#.#...|..#....|.......|.......|"
".###...|.###...|.###...|.###...|.###...|.###...|.#####.|.###...|#####..|#####..|#####..|#####..|.###...|.###...|.###...|.###...|###....|#...#..|.###...|.###...|.###...|.###...|.###...|.......|.###.#.|#...#..|#...#..|#...#..|#...#..|#...#..|#......|###....|"
"#...#..|#...#..|#...#..|#...#..|#...#..|#...#..|#..#...|#...#..|#......|#......|#......|#......|..#....|..#....|..#....|..#....|#..#...|#...#..|#...#..|#...#..|#...#..|#...#..|#...#..|.......|#...#..|#...#..|#...#..|#...#..|#...#..|#...#..|####...|#..#...|"
"#...#..|#...#..|#...#..|#...#..|#...#..|#...#..|#..#...|#......|#......|#......|#......|#......|..#....|..#....|..#....|..#....|#...#..|##..#..|#...#..|#...#..|#...#..|#...#..|#...#..|#...#..|#..##..|#...#..|#...#..|#...#..|#...#..|.#.#...|#...#..|#..#...|"
"#...#..|#...#..|#...#..|#...#..|#...#..|#...#..|######.|#......|####...|####...|####...|####...|..#....|..#....|..#....|..#....|###.#..|#.#.#..|#...#..|#...#..|#...#..|#...#..|#...#..|.#.#...|#.#.#..|#...#..|#...#..|#...#..|#...#..|.#.#...|#...#..|####...|"
"#####..|#####..|#####..|#####..|#####..|#####..|#..#...|#......|#......|#......|#......|#......|..#....|..#....|..#....|..#....|#...#..|#..##..|#...#..|#...#..|#...#..|#...#..|#...#..|..#....|##..#..|#...#..|#...#..|#...#..|#...#..|..#....|#...#..|#...#..|"
"#...#..|#...#..|#...#..|#...#..|#...#..|#...#..|#..#...|#......|#......|#......|#......|#......|..#....|..#....|..#....|..#....|#...#..|#...#..|#...#..|#...#..|#...#..|#...#..|#...#..|.#.#...|#...#..|#...#..|#...#..|#...#..|#...#..|..#....|####...|#...#..|"
"#...#..|#...#..|#...#..|#...#..|#...#..|#...#..|#..#...|#...#..|#......|#......|#......|#......|..#....|..#....|..#....|..#....|#..#...|#...#..|#...#..|#...#..|#...#..|#...#..|#...#..|#...#..|#...#..|#...#..|#...#..|#...#..|#...#..|..#....|#......|##..#..|"
"#...#..|#...#..|#...#..|#...#..|#...#..|#...#..|#..###.|.###...|#####..|#####..|#####..|#####..|.###...|.###...|.###...|.###...|###....|#...#..|.###...|.###...|.###...|.###...|.###...|.......|.###...|.###...|.###...|.###...|.###...|..#....|#......|#.##...|"
".......|.......|.......|.......|.......|.......|.......|..#....|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|"
".......|.......|.......|.......|.......|.......|.......|.#.....|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|"
".......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|"
".......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|"
".......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|"
".......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|"

".......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|"
".......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|#.#....|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|"
".#.....|...#...|..#....|..#.#..|.#.#...|..#....|.......|.......|.#.....|...#...|..#....|.#.#...|.#.....|...#...|..#....|.#.#...|.#.....|..#.#..|.#.....|...#...|..#....|..#.#..|.#.#...|.......|.......|.#.....|...#...|..#....|.#.#...|...#...|#......|.#.#...|"
"..#....|..#....|.#.#...|.#.#...|.#.#...|.#.#...|.......|.......|..#....|..#....|.#.#...|.#.#...|..#....|..#....|.#.#...|.#.#...|#.#....|.#.#...|..#....|..#....|.#.#...|.#.#...|.#.#...|..#....|.......|..#....|..#....|.#.#...|.#.#...|..#....|#......|.#.#...|"
".###...|.###...|.###...|.###...|.###...|.###...|.###...|.###...|.###...|.###...|.###...|.###...|.##....|.##....|.##....|.##....|.###...|####...|.###...|.###...|.###...|.###...|.###...|..#....|..##.#.|#...#..|#...#..|#...#..|#...#..|#...#..|####...|#...#..|"
"....#..|....#..|....#..|....#..|....#..|....#..|..#.#..|#...#..|#...#..|#...#..|#...#..|#...#..|..#....|..#....|..#....|..#....|#...#..|#...#..|#...#..|#...#..|#...#..|#...#..|#...#..|.......|.#..#..|#...#..|#...#..|#...#..|#...#..|#...#..|#...#..|#...#..|"
".####..|.####..|.####..|.####..|.####..|.####..|.##.#..|#......|#####..|#####..|#####..|#####..|..#....|..#....|..#....|..#....|#...#..|#...#..|#...#..|#...#..|#...#..|#...#..|#...#..|#####..|.#.##..|#...#..|#...#..|#...#..|#...#..|#...#..|#...#..|#...#..|"
"#...#..|#...#..|#...#..|#...#..|#...#..|#...#..|#.##...|#......|#......|#......|#......|#......|..#....|..#....|..#....|..#....|#...#..|#...#..|#...#..|#...#..|#...#..|#...#..|#...#..|.......|.##.#..|#...#..|#...#..|#...#..|#...#..|#...#..|#...#..|#...#..|"
"#...#..|#...#..|#...#..|#...#..|#...#..|#...#..|#.#....|#...#..|#...#..|#...#..|#...#..|#...#..|..#....|..#....|..#....|..#....|#...#..|#...#..|#...#..|#...#..|#...#..|#...#..|#...#..|..#....|.#..#..|#...#..|#...#..|#...#..|#...#..|#...#..|#...#..|#...#..|"
".####..|.####..|.####..|.####..|.####..|.####..|.####..|.###...|.###...|.###...|.###...|.###...|.###...|.###...|.###...|.###...|.###...|#...#..|.###...|.###...|.###...|.###...|.###...|..#....|#.##...|.####..|.####..|.####..|.####..|.####..|####...|.####..|"
".......|.......|.......|.......|.......|.......|.......|..#....|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|....#..|#......|....#..|"
".......|.......|.......|.......|.......|.......|.......|.#.....|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.###...|#......|.###...|"
".......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|"
".......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|"
".......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|"
".......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|";

const char* terminus_hex =
".......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|"
".......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|"
".###...|..#....|.###...|.###...|....#..|#####..|.###...|#####..|.###...|.###...|.###...|####...|.###...|###....|#####..|#####..|"
"#...#..|.##....|#...#..|#...#..|...##..|#......|#......|....#..|#...#..|#...#..|#...#..|#...#..|#...#..|#..#...|#......|#......|"
"#..##..|..#....|#...#..|....#..|..#.#..|#......|#......|....#..|#...#..|#...#..|#...#..|#...#..|#......|#...#..|#......|#......|"
"#.#.#..|..#....|...#...|..##...|.#..#..|####...|####...|...#...|.###...|#...#..|#...#..|####...|#......|#...#..|####...|####...|"
"##..#..|..#....|..#....|....#..|#...#..|....#..|#...#..|...#...|#...#..|.####..|#####..|#...#..|#......|#...#..|#......|#......|"
"#...#..|..#....|.#.....|....#..|#####..|....#..|#...#..|..#....|#...#..|....#..|#...#..|#...#..|#......|#...#..|#......|#......|"
"#...#..|..#....|#......|#...#..|....#..|#...#..|#...#..|..#....|#...#..|....#..|#...#..|#...#..|#...#..|#..#...|#......|#......|"
".###...|.###...|#####..|.###...|....#..|.###...|.###...|..#....|.###...|.###...|#...#..|####...|.###...|###....|#####..|#......|"
".......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|"
".......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|"
".......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|"
".......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|"
".......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|"
".......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|.......|";
