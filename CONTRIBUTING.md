# Contributing

## Commit format

Each commit message should record one logical change. In Vee, *conventional commits* are used in the format:

```
<type>(<scope>): <summary>
```

### Types
- **feat** - new feature
- **fix** - bugfix
- **refactor** - code restructuring with no behavioural changes
- **perf** - performance improvement
- **test** - tests
- **docs** - documentation
- **build** - build system or dependencies
- **ci** - CI/CD
- **style** - formatting only
- **chore** - miscellaneous maintenance

### Examples

`refactor(symbols): replace linear lookup with hash maps`
`fix(parser): correctly parse nested generic arguments`
`perf(types): cache method lookup results`

Commit messages should be relatively concise and clear, no more than 50 words.
