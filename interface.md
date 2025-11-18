# YouTube Planner

This document describes the YouTube Planner CLI interface.

## Main Menu

Make a selection by pressing one of the five options: `[1, 2, 3, 4, 5]`

```bash
./youtube_planner

1. View Tasks
2. Edit Tasks
3. Create Task
4. Save
5. Quit
```

## View Tasks Menu

Select the task you want to view details for, or press `0` to return to the previous menu: `[1, 2, 3, ..., n, 0]`

```bash
> 1

1. Task 1
2. Minecraft - 100th day
3. Task 3
....
n. Task n

0. Return to main menu
```

## Task Details

The task details view displays the following information:

- **Title**: Video title
- **Description**: Description for the video
- **Topics**: Topics that the user wants to include in the video. `n` means not completed, while `y` means completed
- **Percent Complete**: Corresponds to the percentage of topics completed
- **Status**: Status of the video, can be `In Progress`, `Completed`, or `Canceled`
- **Updated At**: Corresponds to the date the task was last updated
- **Created At**: Task creation date

**Example:**

```bash
> 2

Title: Minecraft - 100th day
Description: Surviving the 100th day on Minecraft with no fire.
Topics:
- Weapons: n
- Water: y
- Shelter: y
- Food: y
- Other Resources: n
Percent Complete: 60%
Status: In Progress
Updated At: 3/22/2026 22:32
Created At: 3/21/2026 22:32
```

## Edit Tasks

Goes to the View Tasks Menu and lets the user choose which task to edit.

## Edit Task

Will go through one field at a time and let the user modify or skip tasks. Pressing the ESC key will take you back one menu up to `Edit Tasks`.

**Example:**

```bash
> 2

Title: Minecraft - 100th day

> ENTER

Description: Surviving the 100th day on Minecraft with no fire.

> ENTER

Completed Topics: Water, Shelter, Food, Guns

> ENTER

Incomplete Topics: Weapons, Other Resources

> ENTER

Status: In Progress

> ENTER
```

## Create Task

Creates tasks by showing the user the `Edit Task` steps with no initial data.

**Required fields:**

- `Title`
- `Status`

## Save

Saves data to file.

## Quit

Prompts the user if they want to save or quit without saving.
