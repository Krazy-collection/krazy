-- Description: Currently we (only?) use krazy2 for quality checking within KDE.
--              However, work is on the way to integrate another tool which uses
--              a C++ parser so we give the possbility to track results of
--              different tools.
CREATE TABLE tools (
  tool_id     SERIAL PRIMARY KEY,
  name        VARCHAR(50) NOT NULL,
  version     VARCHAR(20),
  description VARCHAR
);

-- Description: We assume that A tool has one or more plugins. For tools with no
--              plugins a plugin with the same name/version as the tool should
--              be added.
CREATE TABLE plugins (
  plugin_id       SERIAL PRIMARY KEY,
  name            VARCHAR(50) NOT NULL,
  version         VARCHAR(20),
  short_desc      VARCHAR,
  long_desc       VARCHAR
);

-- Description: As it might be possible that different versions of a tool uses
--              the same version of some plugin, we link them in this seperate
--              table.
CREATE TABLE tool_plugins (
  tool_id         INTEGER REFERENCES tools(tool_id),
  plugin_id       INTEGER REFERENCES plugins(plugin_id)
);

-- Description: A component within is checked by one or more tools. Each tool is
--              ran for each module of the component and results are stored per
--              plugin of the tool.
CREATE TABLE component_check ( -- A component is checked by one or more tools
  component_check_id SERIAL PRIMARY KEY,
  component          VARCHAR(50),
  revision           VARCHAR(50),
  run_date           TIMESTAMP
);

-- Description: A tool is run for each module. This table stores the tools that
--              are run for a specific component.
CREATE TABLE tool_run (
  tool_run_id        SERIAL PRIMARY KEY,
  component_check_id INTEGER REFERENCES component_check(component_check_id), -- The component that is checked.
  tool_id            INTEGER REFERENCES tools(tool_id),                      -- The tool that is used for this check.
  module             VARCHAR(50) -- A component has one or more modules, the
);                               -- tool is run for each module.

-- Description: Each tool_run has results for every plugin that is executed by
--              the tool. It is assumed that a tool is run on one single file.
--              The number of issues which is found in a specific run can be
--              determined by counting the rows in tool_run_result_issues for a
--              given tool_run_id.
CREATE TABLE tool_run_results (
  tool_run_result_id  SERIAL PRIMARY KEY,
  tool_run_id         INTEGER REFERENCES tool_run(tool_run_id),
  plugin_id           INTEGER REFERENCES plugins(plugin_id), -- The plugin for which the results are stored.
  file_name           VARCHAR(50),  -- The file that is being checked.
  issue_count         INTEGER -- Should be equal to the number of lines in tool_run_result_issues as long
);                            -- as there has not been a new run.

-- I'm not sure about this table, this grows large very quickely. Probably we
-- Only want to store the specific issues for the last run and just keep the count
-- Afterwards.
CREATE TABLE tool_run_result_issues (
  tool_run_result_issue_id SERIAL,
  tool_run_results_id      INTEGER REFERENCES tool_run_results(tool_run_result_id),
  line                     INTEGER,
  expected_value           VARCHAR,
  actual_value             VARCHAR
);