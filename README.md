# CryptRaider

An Unreal Engine first-person puzzle game, where the player is tasked to bring back a valuable artifact hidden inside a mysterious dungeon. This project demonstrates usage of physics system, collisions, exploring lights, using C++ to create custom functionalities by deriving from various Unreal component classes, handling inputs, understanding C++ compilation process.

## Highlights
+ [Level Design](#level-design)
+ [Lighting](#lighting)
+ [Character Handling](#character-handling)
+ [Object Grabbing](#object-grabbing)
+ [Collision](#collision)
+ [Other Custom Classes](#other-custom-classes)
	+ [ActorScaler](#actorscaler)
	+ [Mover](#mover)
	+ [TrapDoor](#trapdoor)
+ [Various Programming Concepts](#various-programming-concepts)
+ [Dependencies](#dependencies)

## **Level Design**

Various assets are put together to create different rooms. Also prop assets are added to give a more dense and real feel to the dungeon. By using different lights in the form of lamps and bonfire make the rooms feel more lively but only lighting to such an extent that the eerie feel of the darkness is retained.

For Example: The first spawning room contains various stone seatings, bonfire, coffin, torches, windows, statues, etc.
![SpawnRoom](https://user-images.githubusercontent.com/43366313/203402303-e483a78a-f099-4a35-804d-5f938b9268f4.png)<br>*Spawn Room*

Some Level Design is done in a modular way to help reuse programmed assets at different places to achieve similar goal.

For Example: A dungeon wall which moves when its trigger component's (custom) collider detects valid trigger item.
This wall is used in first room with hidden mechanic to act as a lift and reused inside crypt to hide a secret room.
![ModularMovableWall](https://user-images.githubusercontent.com/43366313/203402436-8985e46f-c94c-40fb-a983-ccd641332bf4.png)<br>*Movable Wall*

## **Lighting**

Almost every form of lights is used to light the environment using the Lumen Global Illumination and Reflections
1. Directional Light to give a base directional illumination.<br>![DirectionalLight](https://user-images.githubusercontent.com/43366313/203402508-abb2099c-118d-43f2-84c0-00b447015ce3.png)<br>*Directional Light through window*

2. Skylights to indirectly illuminate darker areas of dungeon.<br>![SkyLight](https://user-images.githubusercontent.com/43366313/203402768-4df58800-1489-48d7-8e9d-4453d7c35a4d.png)<br>*Sky Light illuminating dark places with natural light*

3. Point Lights for torches.<br>![PointLight](https://user-images.githubusercontent.com/43366313/203402907-8c947f10-ffe1-4f02-8777-edafa7fad866.png)<br>*Point Light*

4. Spot Lights to highlight grabbable objects.<br>![SpotLight](https://user-images.githubusercontent.com/43366313/203403323-14a18ea8-f58e-4184-80b8-c8d0c89d75d8.png)<br>*Spot Light*

5. Using Emissive materials on particles of bonfire.<br>![EmissiveMaterial](https://user-images.githubusercontent.com/43366313/203402982-152aea05-32f1-4e60-9dc1-f9acccebc74b.png)<br>*Bonfire Particles with Emissive Material*


Apart from this phenomenon of light bleeding happens when assets that are back face culled. The culled faces dont exist to calcuate for blocking/reflecting of light, therefore to avoid this we add adding blockers which are just assets that can act as the faces of the object to block light from entering from back.

![Bleed Blockers](https://user-images.githubusercontent.com/43366313/203404377-138367fc-8370-485d-bcb0-4e48f87af842.png)<br>*Light Bleed Blockers*


## **Character Handling**

Character has ability to move and look around in the environment to solve the puzzles. Unreals Input handling takes 
care of this by binding these actions to keys by overriding ***ACharacter::SetupPlayerInputComponent***
```cpp
void ACryptRaiderCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("PrimaryAction", IE_Pressed, this, &ACryptRaiderCharacter::OnPrimaryAction);

	// Enable touchscreen input
	EnableTouchscreenMovement(PlayerInputComponent);

	// Bind movement events
	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &ACryptRaiderCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &ACryptRaiderCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "Mouse" versions handle devices that provide an absolute delta, such as a mouse.
	// "Gamepad" versions are for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &ACryptRaiderCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &ACryptRaiderCharacter::LookUpAtRate);
}
```

Even touch input is handled by methods
* ACryptRaiderCharacter::BeginTouch
* ACryptRaiderCharacter::EndTouch
* ACryptRaiderCharacter::TouchUpdate
* and by maintaining a touch data under TouchData struct

## **Object Grabbing**

The character also has other abilities like grabbing and placing objects in the dungeon. This is taken care by the ***[Grabber](Source/CryptRaider/Grabber.cpp)*** class.
The core component of this class is PhysicsHandle Component which takes care of the grabbing the object and maintaining its physics properties while grabbed. The object is detected and is deemed grabbable if the sweeped trace collides with an object of desired channel type.

```cpp
// Responsible for firing a sphere raycast from player postion in forward direction 
// and getting the first valid object it hits
bool UGrabber::GetGrabbableInReach(FHitResult& hit) const
{
	FCollisionShape sphere = FCollisionShape::MakeSphere(GrabRadius);
	return GetWorld()->SweepSingleByChannel(
		hit,
		GetComponentLocation(),
		GetComponentLocation() + GetForwardVector() * MaxGrabDistance,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		sphere);
}
```

## **Collision**

Any Static Mesh object in Unreal has a physics body and can have collision responses with other physics bodies in 3 ways namely:
1. Ignore - Will ignore any collisions, and will invoke no events.
2. Overlap - If other physics body is set to block or overlap the channel, then the object will overlap with it and trigger an Overlap event.
3. Block - If other physics body is set to block, then the object collides and triggers Hit event.

These interactions happen depending upon the trace and object channels the collision responses settings of the objects.
Custom Channels can be created to detect custom traces or object types.

## **Other Custom Classes**

Custom classes are created by using inheritence and composition of Unreal classes, like UActorComponen, USceneComponent, UBoxComponent, etc.
Blueprint Callable
Dependency Injection
### **[ActorScaler](Source/CryptRaider/ActorScaler.cpp)**
This component when attached to actor is responsible for scaling it to desired scale value within certain duration.
### **[Mover](Source/CryptRaider/Mover.cpp)**
This component when attached to actor is responsible for moving it to desired location by some offset with within certain duration.
### **[TrapDoor](Source/CryptRaider/TrapDoor.cpp)**
This component can be attached to any actor which has door like properties and a collision area which is responsible for triggering the trapping mechanic. By mentioning the maximun closing angle and duration of closing any actor with such properties can act as a trap door.

Many Unreal provided features used in these classes are:
1. Variable/Data Types:
   1. TArray
2. Macros:
   1. UFUNCTION
3. Math Methods:
   1. FMath::VInterpConstantTo
   2. FMath::RInterpTo
4. Debugging:
   1. DrawDebugLine
   2. DrawDebugSphere
5. Other Useful Methods:
   1. FindComponentByClass
   2. Cast<>

## Various Programming Concepts
1. Inheritence
2. Composition
3. Pointers
4. References
5. Dereferencing **(->)**
6. Const Refenencing method variables
7. Constructors
8. While and For Loops
9. Boolean Logical Operators

## Dependencies
1. [Medieval Dungeon](https://www.unrealengine.com/marketplace/en-US/product/a5b6a73fea5340bda9b8ac33d877c9e2)

      > Not all links may be available
