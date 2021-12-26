package cli

import (
	"flag"

	"dhemery.com/DHE-Modules/panelgen/internal/builder"
)

func Execute() error {
	generateFlag := flag.Bool("g", false, "generate outdated files")
	listFlag := flag.Bool("l", false, "list generated files")
	flag.Parse()
	if *generateFlag {
		if err := builder.Generate(); err != nil {
			return err
		}
	}
	if *listFlag {
		builder.List()
	}
	return nil
}
